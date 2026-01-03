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

void Player::init(Tile* map[MAP_HEIGHT][MAP_WIDTH])
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

	mapCoords = { 16.0f, 12.0f };

	for (int y = 0; y < MAP_HEIGHT; ++y)
	{
		for (int x = 0; x < MAP_WIDTH; ++x)
		{
			m_map[y][x] = map[y][x];
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

	//if (checkForGround())
	{
		//landOnGround(tmpGroundHitBox);
	}

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
				//mapCoords.x -= moveSpeed;
				inputVelocity.x -= moveSpeed;
				srcRect.x = srcRect.w;
			}
			else
			{
				//mapCoords.x += moveSpeed;
				inputVelocity.x += moveSpeed;
				srcRect.x = 0;
			}
		}
		else
		{
			//mapCoords.x += moveSpeed;
			inputVelocity.x += moveSpeed;
			srcRect.x = 0;
			lastKeyPressed = SDL_SCANCODE_D;
		}
	}
	else if (InputManager::isKeyPressed(SDL_SCANCODE_A))
	{
		//mapCoords.x -= moveSpeed;
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
	
	if (velocity.x < 0.0f)
	{
		for(int y = floor(mapCoords.y); y < floor(mapCoords.y) + 1; ++y)
		{
			if (collRectRect(FuturePlayerHitbox, m_map[y][static_cast<int>(floor(mapCoords.x)) - 1]->getTileRect()))
			{

			}
		}
	}
	else if (velocity.x > 0.0f)
	{
		for(int y = floor(mapCoords.y); y < floor(mapCoords.y) + 1; ++y)
		{
			if (collRectRect(FuturePlayerHitbox, m_map[y][static_cast<int>(floor(mapCoords.x)) + 1]->getTileRect()))
			{
			}
		}
	}

}

void Player::landOnGround(SDL_Rect ground)
{
	isOnGround = true;
}

void Player::calculateVelocity()
{
	inputVelocity.x = clamp(inputVelocity.x, -maxInputVelocity.x, maxInputVelocity.x); // clamps input velocity
	inputVelocity.y = clamp(inputVelocity.y, -maxInputVelocity.y, maxInputVelocity.y);

	velocity = gameVelocity + inputVelocity;

	if (isOnGround) // prevents downward velocity when on ground
	{
		velocity.y = max(0.0f, velocity.y); 
	}
}

void Player::applyVelocity()
{
	mapCoords += velocity;
}