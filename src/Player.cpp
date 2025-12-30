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
	stream >> tmp >> hitboxImg >> hitBox.rect;
	stream >> tmp >> hitBox.srcRect;
	stream >> tmp >> playerRenderMultiplier;
	stream >> tmp >> moveSpeed;
	stream >> tmp >> jumpStrength;
	stream >> tmp >> gravity;
	stream >> tmp >> hitboxOffsetSrc;

	texture = loadTexture(playerImg);
	hitBox.texture = loadTexture(hitboxImg);

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

void Player::draw(int2 camOffset)
{
	rect.x = getRealCoords().x - camOffset.x;
	rect.y = getRealCoords().y - camOffset.y;

	drawObject(*this);
	//drawObject(hitBox); Hitbox is a bit shaky, might be due to some camera offset float shenanigans
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

	
	hitBox.rect.w = hitBox.srcRect.w * InputManager::getZoom();
	hitBox.rect.h = hitBox.srcRect.h * InputManager::getZoom();
	hitboxOffset.x = hitboxOffsetSrc.x * InputManager::getZoom();
	hitboxOffset.y = hitboxOffsetSrc.y * InputManager::getZoom();
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
	hitBox.rect.x = rect.x + hitboxOffset.x;
	hitBox.rect.y = rect.y + hitboxOffset.y;
}

void Player::drawHitBox() //for debugging
{
	drawObject(hitBox);
}

void Player::gravityEffect()
{
	velocity.y += gravity;
}

bool Player::checkIfWillHitGround() // how to collide better with ground?
{
	SDL_Rect FuturePlayerHitBox = { hitBox.rect.x + velocity.x, hitBox.rect.y + velocity.y, hitBox.rect.w, hitBox.rect.h };
	
	return collRectRect(FuturePlayerHitBox, tmpGroundHitBox);
}

void Player::landOnGround(SDL_Rect ground)
{
	isOnGround = true;
	
	velocity.y = 0;
	//rect.y = ground.y - hitboxOffset.y - hitBox.rect.h;
	//hitbox.rect.y = rect.y + hitBoxOffset.y;
}
