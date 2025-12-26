#include "Player.h"
#include <Presenter.h>
#include "InputManager.h"


Player::Player()
{
}

Player::~Player()
{
}

void Player::init()
{
	string playerfile = "player.txt", tmp, playerImg, hitboxImg;

	float playerRenderMultiplier;

	fstream stream;

	stream.open(CONFIG_FOLDER + playerfile);

	stream >> tmp >> playerImg;
	stream >> tmp >> srcRect.x >> srcRect.y >> srcRect.w >> srcRect.h;
	stream >> tmp >> rect.x >> rect.y >> rect.w >> rect.h;
	stream >> hitboxImg >> hitBox.x >> hitBox.y >> hitBox.w >> hitBox.h; //UNIFINISHED
	stream >> tmp >> playerRenderMultiplier;
	stream >> tmp >> moveSpeed;
	stream >> tmp >> jumpStrength;
	stream >> tmp >> gravity;
	stream >> tmp >> hitBoxOffset.x >> hitBoxOffset.y;

	texture = loadTexture(playerImg);

	if (texture) {
	    SDL_SetTextureScaleMode(texture, SDL_ScaleModeNearest); // prevents linear filtering for this texture
	}

	stream.close();

	tmpGroundHitBox = { 0, 800, 1920, 100 }; // temporary, for Ground hitbox

	rect.w *= playerRenderMultiplier;
	rect.h *= playerRenderMultiplier;

	hitBox.w *= playerRenderMultiplier;
	hitBox.h *= playerRenderMultiplier;
	hitBoxOffset.x *= playerRenderMultiplier;
	hitBoxOffset.y *= playerRenderMultiplier;

	std::cout << "Player dimention: " << rect.w << std::endl;

	mapCoords = { 16.0f, 12.0f };
}

void Player::update()
{
	move();

	gravityEffect();

	zoomUpdate();
}

void Player::draw(int2 camOffset)
{
	rect.x = getRealCoords().x - camOffset.x;
	rect.y = getRealCoords().y - camOffset.y;

	drawObject(*this);
}

float2 Player::getRealCoords()
{
	return { static_cast<float>(mapCoords.x * TILE_SIZE * InputManager::getZoom()), static_cast<float>(mapCoords.y * TILE_SIZE * InputManager::getZoom()) };
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
}

void Player::move()
{
	moveVertical();

	if ((InputManager::isKeyPressed(SDL_SCANCODE_W) || InputManager::isKeyPressed(SDL_SCANCODE_SPACE)) && isOnGround)
	{
		jump();
	}

	if (checkIfWillHitGround())
	{
		landOnGround(tmpGroundHitBox);
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
				mapCoords.x -= moveSpeed;
				srcRect.x = srcRect.w;
			}
			else
			{
				mapCoords.x += moveSpeed;
				srcRect.x = 0;
			}
		}
		else
		{
			mapCoords.x += moveSpeed;
			srcRect.x = 0;
			lastKeyPressed = SDL_SCANCODE_D;
		}
	}
	else if (InputManager::isKeyPressed(SDL_SCANCODE_A))
	{
		mapCoords.x -= moveSpeed;
		srcRect.x = srcRect.w;
		lastKeyPressed = SDL_SCANCODE_A;
	}
}

void Player::moveSprite()
{
	hitBox.x = rect.x + hitBoxOffset.x;
	hitBox.y = rect.y + hitBoxOffset.y;
}

void Player::gravityEffect()
{
	velocity.y += gravity;
}

bool Player::checkIfWillHitGround() // how to collide better with ground?
{
	SDL_Rect FuturePlayerHitBox = { hitBox.x + velocity.x, hitBox.y + velocity.y, hitBox.w, hitBox.h };
	
	return collRectRect(FuturePlayerHitBox, tmpGroundHitBox);
}

void Player::landOnGround(SDL_Rect ground)
{
	isOnGround = true;
	
	velocity.y = 0;
	rect.y = ground.y - hitBoxOffset.y - hitBox.h;
	hitBox.y = rect.y + hitBoxOffset.y;
}
