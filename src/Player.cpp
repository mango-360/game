#include "Player.h"
#include <Presenter.h>
#include "InputManager.h"
#include "Board.h" // temporary, for Ground access

extern Ground m_ground; // temporary, for Ground access

Player::Player()
{
}

Player::~Player()
{
}

void Player::init()
{
	string playerfile = "player.txt", tmp, playerImg;

	float playerRenderMultiplier;

	fstream stream;

	stream.open(CONFIG_FOLDER + playerfile);

	stream >> tmp >> playerImg;
	stream >> tmp >> srcRect.x >> srcRect.y >> srcRect.w >> srcRect.h;
	stream >> tmp >> rect.x >> rect.y >> rect.w >> rect.h;
	stream >> tmp >> hitBox.x >> hitBox.y >> hitBox.w >> hitBox.h;
	stream >> tmp >> playerRenderMultiplier;
	stream >> tmp >> moveSpeed;
	stream >> tmp >> jumpStrength;
	stream >> tmp >> Gravity;
	stream >> tmp >> hitBoxOffsetX >> hitBoxOffsetY;
	
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
	hitBoxOffsetX *= playerRenderMultiplier;
	hitBoxOffsetY *= playerRenderMultiplier;

	std::cout << "Player dimention: " << rect.w << std::endl;
}

void Player::update()
{
	move();

	gravityEffect();
}

void Player::jump()
{
	VelocityY = -jumpStrength;

	isOnGround = false;
}

void Player::move()
{
	moveVertical();

	if (InputManager::isKeyPressed(SDL_SCANCODE_W) && isOnGround)
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
				rect.x -= moveSpeed;
				srcRect.x = srcRect.w;
			}
			else
			{
				rect.x += moveSpeed;
				srcRect.x = 0;
			}
		}
		else
		{
			rect.x += moveSpeed;
			srcRect.x = 0;
			lastKeyPressed = SDL_SCANCODE_D;
		}
	}
	else if (InputManager::isKeyPressed(SDL_SCANCODE_A))
	{
		rect.x -= moveSpeed;
		srcRect.x = srcRect.w;
		lastKeyPressed = SDL_SCANCODE_A;
	}
}

void Player::moveSprite()
{
	rect.x += VelocityX;
	rect.y += VelocityY;

	hitBox.x = rect.x + hitBoxOffsetX;
	hitBox.y = rect.y + hitBoxOffsetY;
}

void Player::gravityEffect()
{
	VelocityY += Gravity;
}

bool Player::checkIfWillHitGround() // how to collide better with ground?
{
	SDL_Rect FuturePlayerHitBox = { hitBox.x + VelocityX, hitBox.y + VelocityY, hitBox.w, hitBox.h };

	return collRectRect(FuturePlayerHitBox, tmpGroundHitBox);
}

void Player::landOnGround(SDL_Rect ground)
{
	isOnGround = true;
	
	VelocityY = 0;
	rect.y = ground.y - hitBoxOffsetY - hitBox.h;
	hitBox.y = rect.y + hitBoxOffsetY;
}
