#include "Player.h"
#include <Presenter.h>
#include "InputManager.h"
#include <algorithm>
#include <iostream>


Player::Player()
{
}

Player::~Player()
{
}

void Player::init(Tile(*map)[MAP_WIDTH])
{
	string playerfile = "player.txt", tmp, playerImg, hitboxImg;

	fstream stream;

	stream.open(CONFIG_FOLDER + playerfile);

	stream >> tmp >> playerImg;
	stream >> tmp >> srcRect;
	stream >> tmp >> rect;
	stream >> tmp >> hitboxImg >> hitbox.rect;
	stream >> tmp >> moveSpeed;
	stream >> tmp >> maxInputVelocity;
	stream >> tmp >> jumpStrength;
	stream >> tmp >> gravity;

	texture = loadTexture(playerImg);
	hitbox.texture = loadTexture(hitboxImg);

	stream.close();

	for (int y = 0; y < MAP_HEIGHT; ++y)
	{
		for (int x = 0; x < MAP_WIDTH; ++x)
		{
			m_map[y][x] = &map[y][x];
		}
	} 
}

void Player::update()
{
	zoomUpdate();

	move();

	calculateVelocity();

	collision();
	
	addFriction();

	applyVelocity();
}

void Player::draw(float2 camCoords)
{
	SDL_Rect tmpHitboxRect = 
	{
		round((hitbox.rect.x - camCoords.x) * (TILE_SIZE * InputManager::getZoom())),
		round((hitbox.rect.y - camCoords.y) * (TILE_SIZE * InputManager::getZoom())),
		round(hitbox.rect.w * (TILE_SIZE * InputManager::getZoom())),
		round(hitbox.rect.h * (TILE_SIZE * InputManager::getZoom()))
	};


	Drawable tmp = { hitbox.img, hitbox.texture, tmpHitboxRect };
	DrawableWithSrc tmpPlayer = { tmp, srcRect };
	tmpPlayer.rect = 
	{
		tmp.rect.x - (rect.w / 2 - tmp.rect.w / 2),
		tmp.rect.y - (rect.h / 2 - tmp.rect.h / 2),
		rect.w,
		rect.h
	};
	tmpPlayer.texture = texture;

	drawObject(tmpPlayer);
	//drawObject(tmp); //hitbox
}

int2 Player::getRealCoords()
{
	return { static_cast<int>(hitbox.rect.x * TILE_SIZE * InputManager::getZoom()), static_cast<int>(hitbox.rect.y * TILE_SIZE * InputManager::getZoom()) };
}

void Player::jump()
{
	if(isOnGround) velocity.y = -jumpStrength;

	isOnGround = false;
}

void Player::zoomUpdate()
{
	rect.w = srcRect.w * InputManager::getZoom();
	rect.h = srcRect.h * InputManager::getZoom();
}

void Player::move()
{

	moveVertical();

	if ((InputManager::isKeyPressed(SDL_SCANCODE_W) || InputManager::isKeyPressed(SDL_SCANCODE_SPACE)) && isOnGround)
	{
		jump();
	}
}

void Player::moveVertical()
{
	if (InputManager::isKeyPressed(SDL_SCANCODE_D))
	{
		if (InputManager::isKeyPressed(SDL_SCANCODE_A))
		{
			if (lastKeyPressed == SDL_SCANCODE_D)
			{
				inputVelocity.x -= moveSpeed;
				srcRect.x = srcRect.w;
			}
			else
			{
				inputVelocity.x += moveSpeed;
				srcRect.x = 0;
			}
		}
		else
		{
			inputVelocity.x += moveSpeed;
			srcRect.x = 0;
			lastKeyPressed = SDL_SCANCODE_D;
		}
	}
	else 
	{
		if (InputManager::isKeyPressed(SDL_SCANCODE_A))
		{
			inputVelocity.x -= moveSpeed;
			srcRect.x = srcRect.w;
			lastKeyPressed = SDL_SCANCODE_A;
		}
		else inputVelocity.x = 0;
	}
}

void Player::drawHitBox(float2 camCoords) //for debugging
{
	SDL_Rect tmpHitboxRect =
	{
		round((hitbox.rect.x - camCoords.x) * (TILE_SIZE * InputManager::getZoom())),
		round((hitbox.rect.y - camCoords.y) * (TILE_SIZE * InputManager::getZoom())),
		round(hitbox.rect.w * (TILE_SIZE * InputManager::getZoom())),
		round(hitbox.rect.h * (TILE_SIZE * InputManager::getZoom()))
	};


	Drawable tmp = { hitbox.img, hitbox.texture, tmpHitboxRect };

	drawObject(tmp);
}

void Player::collision()
{
	bool hitsGround = false;
	friction = { 0, 0 };

	float2 cp, cn;
	float t = 0, min_t = INFINITY, prevFriction = 0;
	vector<pair<int2, float>> collsList;

	// Work out collision point, add it to vector along with rect ID
	for (int i = floor(hitbox.rect.y) - 1; i <= ceil(hitbox.rect.y + hitbox.rect.h); ++i)
	{
		for (int j = floor(hitbox.rect.x) - 1; j <= ceil(hitbox.rect.x + hitbox.rect.w); ++j)
		{
			if (m_map[i][j]->getTileType() == TILE_TYPE::NONE_TYPE) continue;

			if (DynamicRectVsRect(&hitbox.rect, velocity, m_map[i][j]->getTileGridRect(), cp, cn, t))
			{
				/*cout << "Player at: " << floor(hitbox.rect.x) << ", " << floor(hitbox.rect.y) << endl;
				cout << "Checking block at: " << j << ", " << i << endl;
				cout << "Contact normal: " << cn << endl;*/

				collsList.push_back({ {i, j}, t });

				if (cn.y == -1)
				{
					hitsGround = true;

					if(m_map[i][j]->getFriction() > prevFriction) calculateFriction({ i, j });

					prevFriction = m_map[i][j]->getFriction();
				}
			}
		}
	}

	// Do the sort
	sort(collsList.begin(), collsList.end(), [](const pair<int2, float>& a, const pair<int2, float>& b)
		{
			return a.second < b.second;
		});

	// Now resolve the collision in correct order 
	for (auto j : collsList) 
	{
		// Avoid taking the address of a temporary returned by getTileGridRect():
		SDL_FRect tileRect = m_map[j.first.x][j.first.y]->getTileGridRect();
		ResolveDynamicRectVsRect(&hitbox.rect, velocity, &tileRect);
	}

	isOnGround = hitsGround;
}

void Player::calculateFriction(int2 coords)
{
	friction.x = min(abs(velocity.x), abs(gravity.y * m_map[coords.x][coords.y]->getFriction()));

	if (velocity.x > 0) friction.x *= -1;
	if (velocity.y > 0) friction.y *= -1;
}

void Player::calculateVelocity()
{
	velocity += calculateNetForce();

	if(velocity.x > 0) inputVelocity.x = clamp(inputVelocity.x, -maxInputVelocity.x, max(0.0f, maxInputVelocity.x - velocity.x)); // clamps x input velocity
	else	inputVelocity.x = clamp(inputVelocity.x, min(0.0f, -maxInputVelocity.x - velocity.x), maxInputVelocity.x); 
	//inputVelocity.y = clamp(inputVelocity.y, -maxInputVelocity.y, maxInputVelocity.y); // clamps y input velocity

	velocity += inputVelocity;

	if(velocity.x != 0 && abs(velocity.x) < 0.001f) //????????????????????????
	{
		velocity.x = 0;
		cout << "Player velocity x reset at " << velocity.x << endl;
	}
	if(velocity.y != 0 && abs(velocity.y) < 0.001f)
	{
		velocity.y = 0;
		cout << "Player velocity y reset at " << velocity.y << endl;
	}
}

float2 Player::calculateNetForce()
{
	return gravity;
}

void Player::addFriction()
{
	velocity += friction;
}

void Player::applyVelocity()
{
	hitbox.rect.x += velocity.x;
	hitbox.rect.y += velocity.y;
	cout << "Player velocity: " << velocity.x << ", " << velocity.y << endl;
}