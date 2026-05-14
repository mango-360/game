#include "Entity.h"
#include "Presenter.h"
#include "InputManager.h"

Tile* Entity::m_map[MAP_HEIGHT][MAP_WIDTH] = { nullptr };

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::init(Tile(*map)[MAP_WIDTH], string configFile)
{
	string tmp, EntityImg, hitboxImg;

	fstream stream;

	stream.open(CONFIG_FOLDER + configFile);

	stream >> tmp >> EntityImg;
	stream >> tmp >> srcRect;
	stream >> tmp >> rect;
	stream >> tmp >> hitboxImg >> hitbox.rect;
	stream >> tmp >> tmp; // temporary to sync with player config file, as moveSpeed is not used in Entity
	stream >> tmp >> maxInputVelocity;
	stream >> tmp >> jumpStrength;

	EntityImg = "mob.bmp"; // temporary to bypass player config file, as texture is not used in Entity
	texture = loadTexture(EntityImg);
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

void Entity::update()
{
	updatePrePhysics();
	updatePostPhysics();
}

void Entity::updatePrePhysics()
{
	zoomUpdate();

	move();

	calculateVelocity();
}

void Entity::updatePostPhysics()
{
	addFriction();

	applyVelocity();

	stopOutOfBounds();
}

void Entity::draw(float2 camCoords)
{
	SDL_Rect tmpHitboxRect =
	{
		round((hitbox.rect.x - camCoords.x) * (TILE_SIZE * InputManager::getZoom())),
		round((hitbox.rect.y - camCoords.y) * (TILE_SIZE * InputManager::getZoom())),
		round(hitbox.rect.w * (TILE_SIZE * InputManager::getZoom())),
		round(hitbox.rect.h * (TILE_SIZE * InputManager::getZoom()))
	};

	Drawable tmp = { hitbox.img, hitbox.texture, tmpHitboxRect };
	DrawableWithSrc tmpEntity = { tmp, srcRect };
	tmpEntity.rect =
	{
		tmp.rect.x - (rect.w / 2 - tmp.rect.w / 2), // center the entity sprite on the hitbox
		tmp.rect.y - (rect.h / 2 - tmp.rect.h / 2),
		rect.w,
		rect.h
	};
	tmpEntity.texture = texture;

	drawObject(tmpEntity);
	//drawObject(tmp); //hitbox
}

int2 Entity::getIntCoords()
{
	return { static_cast<int>(hitbox.rect.x), static_cast<int>(hitbox.rect.y) };
}

void Entity::jump()
{
	if (isOnGround) velocity.y = -jumpStrength;

	isJumping = true;
}

void Entity::zoomUpdate()
{
	rect.w = TILE_SIZE * InputManager::getZoom();
	rect.h = TILE_SIZE * InputManager::getZoom();
}

void Entity::move()
{

	moveVertical();

	if ((InputManager::isKeyPressed(SDL_SCANCODE_W) || InputManager::isKeyPressed(SDL_SCANCODE_SPACE)) && isOnGround)
	{
		jump();
	}
}

void Entity::moveVertical()
{
}

void Entity::drawHitBox(float2 camCoords) //for debugging
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

void Entity::resolveCollision(SDL_FRect tileRect)
{
	ResolveDynamicRectVsRect(&hitbox.rect, velocity, &tileRect);
}

void Entity::calculateFriction(float frictionValue)
{
	friction.x = min(abs(velocity.x), abs(GRAVITY.y * frictionValue));
	friction.y = min(abs(velocity.y), abs(GRAVITY.y * frictionValue * 0.1f));

	if (velocity.x > 0) friction.x *= -1;
	if (velocity.y > 0) friction.y *= -1;
}

void Entity::calculateVelocity()
{
	velocity += calculateNetForce();

	if (velocity.x != 0 && abs(velocity.x) < 0.001f) velocity.x = 0;
	if (velocity.y != 0 && abs(velocity.y) < 0.001f) velocity.y = 0;
}

void Entity::addFriction()
{
	velocity += friction;
}

float2 Entity::calculateNetForce()
{
	return GRAVITY;
}

void Entity::applyVelocity()
{
	hitbox.rect.x += velocity.x;
	hitbox.rect.y += velocity.y;
}

void Entity::stopOutOfBounds()
{
	if (hitbox.rect.x < 0)
	{
		hitbox.rect.x = 0;
		velocity.x = 0;
	}
	else if (hitbox.rect.x + hitbox.rect.w > MAP_WIDTH)
	{
		hitbox.rect.x = MAP_WIDTH - hitbox.rect.w;
		velocity.x = 0;
	}
	if (hitbox.rect.y < 0)
	{
		hitbox.rect.y = 0;
		velocity.y = 0;
	}
	else if (hitbox.rect.y + hitbox.rect.h > MAP_HEIGHT)
	{
		hitbox.rect.y = MAP_HEIGHT - hitbox.rect.h;
		velocity.y = 0;
	}
}
