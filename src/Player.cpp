#include "Player.h"
#include <Presenter.h>
#include "InputManager.h"
#include <algorithm>


Player::Player()
{
}

Player::~Player()
{
}

void Player::init(Tile(*map)[MAP_WIDTH])
{
	string playerfile = "player.txt", tmp, playerImg, hitboxImg;

	float playerRenderMultiplier;

	fstream stream;

	stream.open(CONFIG_FOLDER + playerfile);

	stream >> tmp >> playerImg;
	stream >> tmp >> srcRect;
	stream >> tmp >> rect;
	stream >> tmp >> hitboxImg >> hitbox.rect;
	stream >> tmp >> hitbox.srcRect;
	stream >> tmp >> playerRenderMultiplier;
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

	tmpGroundHitBox = { 0, 800, 1920, 100 }; // temporary, for Ground hitbox

	mapCoords = { 16.0f, 5.0f };

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

	gravityEffect();

	calculateVelocity();
	applyVelocity();

	//cout << "Player map coords: " << mapCoords.x << ", " << mapCoords.y << endl;
}

void Player::draw(float2 camCoords)
{
	rect.x = (mapCoords.x - camCoords.x) * (TILE_SIZE * InputManager::getZoom());
	rect.y = (mapCoords.y - camCoords.y) * (TILE_SIZE * InputManager::getZoom());

	drawObject(*this);
	drawObject(hitbox);
}

int2 Player::getRealCoords()
{
	return { static_cast<int>(mapCoords.x * TILE_SIZE * InputManager::getZoom()), static_cast<int>(mapCoords.y * TILE_SIZE * InputManager::getZoom()) };
}

void Player::jump()
{
	velocity.y = -jumpStrength;

	isOnGround = false;
}

void Player::zoomUpdate()
{
	rect.w = srcRect.w * InputManager::getZoom();
	rect.h = srcRect.h * InputManager::getZoom();

	
	hitbox.rect.w = hitbox.srcRect.w * InputManager::getZoom();
	hitbox.rect.h = hitbox.srcRect.h * InputManager::getZoom();
}

void Player::move()
{
	moveVertical();

	if ((InputManager::isKeyPressed(SDL_SCANCODE_W) || InputManager::isKeyPressed(SDL_SCANCODE_SPACE)) && isOnGround)
	{
		jump();
	}

	checkForGround();

	moveSprite();
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

void Player::moveSprite()
{
	hitbox.rect.x = rect.x + rect.w / 2 - hitbox.rect.w / 2;
	hitbox.rect.y = rect.y + rect.h / 2 - hitbox.rect.h / 2;
}

void Player::drawHitBox() //for debugging
{
	drawObject(hitbox);
}

void Player::gravityEffect()
{
	gameVelocity.y += gravity;
}

void Player::checkForGround() // how to collide better with ground?
{
	SDL_Rect FuturePlayerHitbox = { hitbox.rect.x + velocity.x, hitbox.rect.y + velocity.y, hitbox.rect.w, hitbox.rect.h };
	
	const int x = static_cast<int>(floor(mapCoords.x));

	if (velocity.x < 0.0f)
	{
		for (int y = floor(mapCoords.y); y <= floor(mapCoords.y) + 1; ++y)
		{
			if (collRectRect(FuturePlayerHitbox, m_map[y][x - 1]->getTileRect()))
			{
				cout << "COLLIDED LEFT" << endl;

				if (FuturePlayerHitbox.y + FuturePlayerHitbox.h - m_map[y][x - 1]->getTileRect().y >  // y diff
					m_map[y][x - 1]->getTileRect().x + m_map[y][x - 1]->getTileRect().w - FuturePlayerHitbox.x) // x diff - THIS IS CORRECT
				{
					isOnWall = true;
					isLeftWall = true;
					landOnWall({ x - 1, y }, isLeftWall);
				}
				else
				{
					landOnGround({ x - 1, y });
				}
			}
		}
	}
	else if (velocity.x > 0.0f)
	{
		for(int y = floor(mapCoords.y); y <= floor(mapCoords.y) + 1; ++y)
		{
			if (collRectRect(FuturePlayerHitbox, m_map[y][x + 1]->getTileRect()))
			{
				cout << "COLLIDED RIGHT" << endl;

				if (FuturePlayerHitbox.y + FuturePlayerHitbox.h - m_map[y][x + 1]->getTileRect().y >			// y diff
					FuturePlayerHitbox.x + FuturePlayerHitbox.w - m_map[y][x + 1]->getTileRect().x)				// x diff
				{
					isOnWall = true;
					isLeftWall = false;

					landOnWall({ x + 1, y }, isLeftWall);

					cout << "LAND ON WALL RIGHT" << endl;
				}
				else
				{
					landOnGround({ x + 1, y });

					cout << "LAND ON GROUND MOVING RIGHT" << endl;
				}
			}
		}
	}

	if (velocity.x == 0.0f)
	{

		if(collRectRect(FuturePlayerHitbox, m_map[static_cast<int>(floor(mapCoords.y) + 1)][x]->getTileRect()))
		{
			landOnGround({ x, static_cast<int>(floor(mapCoords.y) + 1) });
		}
	}

}

void Player::landOnGround(int2 ground)
{
	mapCoords.y = ground.y - ((static_cast<float>((rect.h - hitbox.rect.h)) / 2.0f + hitbox.rect.h) / (TILE_SIZE * InputManager::getZoom())); // align player on top of ground

	isOnGround = true;
}

void Player::landOnWall(int2 wall, bool isLeftWall)
{
	if (isLeftWall)
	{
		mapCoords.x = wall.x + 1 - static_cast<float>((rect.w - hitbox.rect.w)) / 2.0f; // align player to the right of the wall
	}
	else
	{
		mapCoords.x = wall.x - ((static_cast<float>((rect.w - hitbox.rect.w)) / 2.0f + hitbox.rect.w) / (TILE_SIZE * InputManager::getZoom())); // align player to the left of the wall
	}
}

void Player::calculateVelocity()
{
	inputVelocity.x = clamp(inputVelocity.x, -maxInputVelocity.x, maxInputVelocity.x); // clamps input velocity
	inputVelocity.y = clamp(inputVelocity.y, -maxInputVelocity.y, maxInputVelocity.y);

	velocity = gameVelocity + inputVelocity;

	if (!(InputManager::isKeyPressed(SDL_SCANCODE_W) || InputManager::isKeyPressed(SDL_SCANCODE_SPACE))) inputVelocity.y = 0.0f; // resets jump input velocity when not jumping

	if (isOnGround) // prevents downward velocity when on ground
	{
		velocity.y = min(0.0f, velocity.y);
	}

	if (isOnWall)
	{
		if (isLeftWall)
		{
			velocity.x = max(0.0f, velocity.x); // prevents leftward velocity when on left wall

			//cout << "IS ON LEFT WALL" << endl;
		}
		else
		{
			velocity.x = min(0.0f, velocity.x); // prevents rightward velocity when on right wall

			//cout << "IS ON RIGHT WALL" << endl;
		}
	}
}

void Player::applyVelocity()
{
	mapCoords += velocity;
}