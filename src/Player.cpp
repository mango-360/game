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
	string playerfile = "player.txt", tmp, playerImg;

	fstream stream;

	stream.open(CONFIG_FOLDER + playerfile);

	stream >> tmp >> playerImg;
	stream >> tmp >> srcRect.x >> srcRect.y >> srcRect.w >> srcRect.h;
	stream >> tmp >> rect.x >> rect.y >> rect.w >> rect.h;
	
	texture = loadTexture(playerImg);

	stream.close();
}

void Player::update()
{
	if (InputManager::isKeyPressed(SDL_SCANCODE_D)) // doesn't work properly when both keys are pressed
	{
		if(InputManager::isKeyPressed(SDL_SCANCODE_A))
		{
			if(lastKeyPressed == SDL_SCANCODE_D)
			{
				rect.x -= 5;
				srcRect.x = srcRect.w;
			}
			else
			{
				rect.x += 5;
				srcRect.x = 0;
			}
		}
		else
		{
			rect.x += 5;
			srcRect.x = 0;
			lastKeyPressed = SDL_SCANCODE_D;
		}
	}
	if (InputManager::isKeyPressed(SDL_SCANCODE_A))
	{
		rect.x -= 5;
		srcRect.x = srcRect.w;
		lastKeyPressed = SDL_SCANCODE_A;
	}
}