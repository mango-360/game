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
	stream >> tmp >> srcRect;
	stream >> tmp >> rect;
	stream >> tmp >> hitboxImg >> hitbox.rect;
	stream >> tmp >> hitbox.srcRect;
	stream >> tmp >> playerRenderMultiplier;
	stream >> tmp >> moveSpeed;
	stream >> tmp >> jumpStrength;
	stream >> tmp >> gravity;

	texture = loadTexture(playerImg);
	hitbox.texture = loadTexture(hitboxImg);

	if (texture) {
	    SDL_SetTextureScaleMode(texture, SDL_ScaleModeNearest); // prevents linear filtering for this texture
	}

	stream.close();

	tmpGroundHitBox = { 0, 800, 1920, 100 }; // temporary, for Ground hitbox

	/*rect.w *= playerRenderMultiplier;
	rect.h *= playerRenderMultiplier;

	hitBox.rect.w *= playerRenderMultiplier;
	hitBox.rect.h *= playerRenderMultiplier;
	hitBoxOffset.x *= playerRenderMultiplier;
	hitBoxOffset.y *= playerRenderMultiplier;*/

	std::cout << "Player dimention: " << rect.w << std::endl;

	mapCoords = { 16.0f, 12.0f };
}

void Player::update()
{
	zoomUpdate();

	move();

	gravityEffect();
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
	hitbox.rect.x = rect.x + rect.w / 2 - hitbox.rect.w / 2;
	hitbox.rect.y = rect.y + rect.h / 2 - hitbox.rect.h / 2;
}

void Player::drawHitBox() //for debugging
{
	drawObject(hitbox);
}

void Player::gravityEffect()
{
	velocity.y += gravity;
}

bool Player::checkIfWillHitGround() // how to collide better with ground?
{
	SDL_Rect FuturePlayerHitBox = { hitbox.rect.x + velocity.x, hitbox.rect.y + velocity.y, hitbox.rect.w, hitbox.rect.h };
	
	return collRectRect(FuturePlayerHitBox, tmpGroundHitBox);
}

void Player::landOnGround(SDL_Rect ground)
{
	isOnGround = true;
	
	velocity.y = 0;
	//rect.y = ground.y - hitboxOffset.y - hitBox.rect.h;
	//hitbox.rect.y = rect.y + hitBoxOffset.y;
}
