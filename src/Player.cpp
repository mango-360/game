#include "Player.h"
#include <Presenter.h>
#include "InputManager.h"
#include "Projectile.h"


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

	landingStartSpriteFrame = 11;
	NoJumpLandingSpriteFrame = 12;
}

void Player::update()
{
	updatePrePhysics();
	updatePostPhysics();
}

void Player::updatePrePhysics()
{
	zoomUpdate();

	shoot();

	move();
	animateJump();
	animateFall();
	animateLand();

	calculateVelocity();
}

void Player::updatePostPhysics()
{
	addFriction();

	applyVelocity();

	stopOutOfBounds();

	countFramesOnGround();
}

void Player::setProjectileSpawner(std::function<void(std::unique_ptr<Projectile>)> spawner)
{
	m_spawnProjectile = std::move(spawner);
}

void Player::shoot()
{
    if ((InputManager::isKeyClicked(SDL_SCANCODE_P) || InputManager::isMousePressed()) && m_spawnProjectile)
    {
        auto projectile = std::make_unique<Projectile>();
        projectile->init(this);
        m_spawnProjectile(std::move(projectile)); // hand ownership to Board via callback
    }
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
				srcRect.y = srcRect.h;
			}
			else
			{
				inputVelocity.x += moveSpeed;
				srcRect.y = 0;
			}
		}
		else
		{
			inputVelocity.x += moveSpeed;
			srcRect.y = 0;
			lastKeyPressed = SDL_SCANCODE_D;
		}
	}
	else 
	{
		if (InputManager::isKeyPressed(SDL_SCANCODE_A))
		{
			inputVelocity.x -= moveSpeed;
			srcRect.y = srcRect.h;
			lastKeyPressed = SDL_SCANCODE_A;
		}
		else inputVelocity.x = 0;
	}
}

void Player::animateJump()
{
	if (isJumping)
	{
		if (isOnGround) srcRect.x = srcRect.w;

		else
		{
			float phase = velocity.y / jumpStrength; // from -1 to 1 and above
			
				 if (phase < -0.83) srcRect.x = srcRect.w * 2;
			else if (phase < -0.66) srcRect.x = srcRect.w * 3;
			else if (phase < -0.50) srcRect.x = srcRect.w * 4;
			else if (phase < -0.33) srcRect.x = srcRect.w * 5;
			else if (phase < -0.16) srcRect.x = srcRect.w * 6;

			else if (phase < 0.25) srcRect.x = srcRect.w * 7;
			else if (phase < 0.50) srcRect.x = srcRect.w * 8;
			else if (phase < 0.75) srcRect.x = srcRect.w * 9;
			else				   srcRect.x = srcRect.w * 10;
		}
		
		landingStartSpriteFrame = 11;
	}
}

void Player::animateFall()
{
	if (!isOnGround && !isJumping)
	{
		float phase = velocity.y / jumpStrength;

		if		(phase < 0.40) srcRect.x = 0;
		else if (phase < 0.75) srcRect.x = srcRect.w * 9;
		else				   srcRect.x = srcRect.w * 10;
	}
}

void Player::animateLand()
{
	if (isOnGround && !isJumping)
	{
		int landingSpriteFrame = framesOnGround / LANDING_SPRITE_FRAME_DURATION;

		if (landingSpriteFrame + landingStartSpriteFrame > 15) srcRect.x = 0;
		else srcRect.x = srcRect.w * (landingStartSpriteFrame + landingSpriteFrame);
	}
}

void Player::calculateVelocity()
{
	velocity += calculateNetForce();

	if(velocity.x > 0) inputVelocity.x = clamp(inputVelocity.x, -maxInputVelocity.x, max(0.0f, maxInputVelocity.x - velocity.x)); // clamps x input velocity
	else	inputVelocity.x = clamp(inputVelocity.x, min(0.0f, -maxInputVelocity.x - velocity.x), maxInputVelocity.x); 
	//inputVelocity.y = clamp(inputVelocity.y, -maxInputVelocity.y, maxInputVelocity.y); // clamps y input velocity

	velocity += inputVelocity;

	if(velocity.x != 0 && abs(velocity.x) < 0.001f) velocity.x = 0;
	if(velocity.y != 0 && abs(velocity.y) < 0.001f) velocity.y = 0;
}

void Player::countFramesOnGround()
{
	if (!isOnGround) framesOnGround = 0;
	else framesOnGround++;
}
