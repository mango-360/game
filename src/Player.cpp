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
}

void Player::update()
{
	zoomUpdate();

	shoot();

	move();
	animateJump();
	animateFall();
	animateLand();

	calculateVelocity();

	collision();
	
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
	if (InputManager::isKeyClicked(SDL_SCANCODE_P) || InputManager::isMousePressed())
	{
		auto projectile = std::make_unique<Projectile>();
		projectile->init(this);

		if (m_spawnProjectile) m_spawnProjectile(std::move(projectile)); // hand ownership to board via the callback
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
	if (isOnGround) isJumping = false;
	else if (!isJumping) landingStartSpriteFrame = 12;
}

void Player::calculateFriction(int2 coords)
{
	friction.x = min(abs(velocity.x), abs(GRAVITY.y * m_map[coords.x][coords.y]->getFriction()));

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

	if(velocity.x != 0 && abs(velocity.x) < 0.001f) velocity.x = 0;
	if(velocity.y != 0 && abs(velocity.y) < 0.001f) velocity.y = 0;
}

void Player::addFriction()
{
	velocity += friction;
}

void Player::countFramesOnGround()
{
	if (!isOnGround) framesOnGround = 0;
	else framesOnGround++;
}
