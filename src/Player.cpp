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

	if (texture) {
	    SDL_SetTextureScaleMode(texture, SDL_ScaleModeNearest); // prevents linear filtering for this texture
	}

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

	calculateVelocity();

	move();
	applyVelocity();

	//cout << "Player map coords: " << hitbox.rect.x << ", " << hitbox.rect.y << endl;
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

	drawObject(tmp);
	drawObject(tmpPlayer);
}

int2 Player::getRealCoords()
{
	return { static_cast<int>(hitbox.rect.x * TILE_SIZE * InputManager::getZoom()), static_cast<int>(hitbox.rect.y * TILE_SIZE * InputManager::getZoom()) };
}

void Player::jump()
{
	inputVelocity.y = -jumpStrength;

	isOnGround = false;

	cout << "JUMP!" << endl;
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

	collision();
	//checkForGround();

	//cout << "Player map coords: " << hitbox.rect.x << ", " << hitbox.rect.y << endl;
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
	else if (InputManager::isKeyPressed(SDL_SCANCODE_A))
	{
		inputVelocity.x -= moveSpeed;
		srcRect.x = srcRect.w;
		lastKeyPressed = SDL_SCANCODE_A;
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

	float2 cp, cn;
	float t = 0, min_t = INFINITY;
	vector<pair<int2, float>> collsList;

	// Work out collision point, add it to vector along with rect ID
	for (int i = floor(hitbox.rect.y) - 1; i <= ceil(hitbox.rect.y + hitbox.rect.h); ++i)
	{
		for (int j = floor(hitbox.rect.x) - 1; j <= ceil(hitbox.rect.x + hitbox.rect.w); ++j)
		{
			if (DynamicRectVsRect(&hitbox.rect, velocity, m_map[i][j]->getTileGridRect(), cp, cn, t))
			{
				collsList.push_back({ {i, j}, t });
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
		if (ResolveDynamicRectVsRect(&hitbox.rect, velocity, &tileRect)) hitsGround = true;
		cout << "velocity after resolve: " << velocity.x << ", " << velocity.y << endl;
	}

	isOnGround = hitsGround;
}

void Player::calculateVelocity()
{
	inputVelocity.x = clamp(inputVelocity.x, -maxInputVelocity.x, maxInputVelocity.x); // clamps input velocity
	inputVelocity.y = clamp(inputVelocity.y, -maxInputVelocity.y, maxInputVelocity.y);

	gameVelocity.y += inputVelocity.y; // adds input velocity to game velocity
	gameVelocity.y += gravity; // applies gravity to game velocity

	velocity = gameVelocity;
	velocity.x += inputVelocity.x;

	//if (!(InputManager::isKeyPressed(SDL_SCANCODE_W) || InputManager::isKeyPressed(SDL_SCANCODE_SPACE))) inputVelocity.y = 0.0f; // resets jump input velocity when not jumping

	if (isOnGround) // prevents downward velocity when on ground
	{
		gameVelocity.y = 0.0f;
	}
	else
	{
		inputVelocity.y = 0.0f; // resets jump input velocity when not on ground
	}

	//if (isOnWall)
	//{
	//	if (isLeftWall)
	//	{
	//		velocity.x = max(0.0f, velocity.x); // prevents leftward velocity when on left wall

	//		//cout << "IS ON LEFT WALL" << endl;
	//	}
	//	else
	//	{
	//		velocity.x = min(0.0f, velocity.x); // prevents rightward velocity when on right wall

	//		//cout << "IS ON RIGHT WALL" << endl;
	//	}
	//}
}

void Player::applyVelocity()
{
	//cout << "Velocity: " << velocity.x << ", " << velocity.y << endl;

	hitbox.rect.x += velocity.x;
	hitbox.rect.y += velocity.y;
}