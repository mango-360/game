#include "Projectile.h"
#include "InputManager.h"
#include "Presenter.h"
#include "Drop.h"

Projectile::Projectile()
{
}

Projectile::~Projectile()
{

}

void Projectile::init(Entity* owner)
{
	m_owner = owner;

	netForce = { 0.0f, 0.0f };

	string tmp;
	
	float hitboxWidth, hitboxHeight;

	fstream stream;

	stream.open(CONFIG_FOLDER + "projectile.txt");

	stream >> tmp >> img;
	stream >> tmp >> hitbox.img;
	stream >> tmp >> hitboxWidth >> hitboxHeight;
	stream >> tmp >> srcRect;
	stream >> tmp >> damage;
	stream >> tmp >> velocity;

	stream.close();

	hitbox.rect = { m_owner->getMapRect().x + m_owner->getMapRect().w , m_owner->getMapRect().y + m_owner->getMapRect().h / 2 - hitboxHeight * PROJECTILE_SIZE / 2,
					hitboxWidth * PROJECTILE_SIZE, hitboxHeight * PROJECTILE_SIZE };

	hitbox.texture = loadTexture(hitbox.img);
	texture = loadTexture(img);

	if (m_owner->velocity.x < 0 || owner->srcRect.y == owner->srcRect.h)
	{
		velocity.x *= -1;
		srcRect.x = srcRect.w;
		hitbox.rect.x = m_owner->getMapRect().x - hitbox.rect.w;
	}

	velocity += m_owner->velocity; 
}

void Projectile::update()
{
	zoomUpdate();

	applyVelocity();

	stopOutOfBounds();
}

void Projectile::draw(float2 camCoords)
{
	SDL_Rect tmpHitboxRect =
	{
		round((hitbox.rect.x - camCoords.x) * (TILE_SIZE * InputManager::getZoom())),
		round((hitbox.rect.y - camCoords.y) * (TILE_SIZE * InputManager::getZoom())),
		round(hitbox.rect.w * (TILE_SIZE * InputManager::getZoom())),
		round(hitbox.rect.h * (TILE_SIZE * InputManager::getZoom()))
	};

	Drawable tmp = { hitbox.img, hitbox.texture, tmpHitboxRect };
	DrawableWithSrc tmpProjectile = { tmp, srcRect };
	tmpProjectile.rect =
	{
		tmp.rect.x - (rect.w / 2 - tmp.rect.w / 2), // center the projectile sprite on the hitbox
		tmp.rect.y - (rect.h / 2 - tmp.rect.h / 2),
		rect.w,
		rect.h
	};
	tmpProjectile.texture = texture;

	drawObject(tmpProjectile);
	//drawObject(tmp); //hitbox
}

void Projectile::zoomUpdate()
{
	rect.w = TILE_SIZE * InputManager::getZoom() * PROJECTILE_SIZE;
	rect.h = TILE_SIZE * InputManager::getZoom() * PROJECTILE_SIZE;
}

void Projectile::calculateVelocity()
{
	velocity += GRAVITY;

	if (velocity.x != 0 && abs(velocity.x) < 0.001f) velocity.x = 0;
	if (velocity.y != 0 && abs(velocity.y) < 0.001f) velocity.y = 0;
}

void Projectile::applyVelocity()
{
	hitbox.rect.x += velocity.x;
	hitbox.rect.y += velocity.y;
}

void Projectile::stopOutOfBounds()
{
	if (hitbox.rect.x < 0 || hitbox.rect.x + hitbox.rect.w > MAP_WIDTH * TILE_SIZE ||
		hitbox.rect.y < 0 || hitbox.rect.y + hitbox.rect.h > MAP_HEIGHT * TILE_SIZE)
	{
		isAlive = false;
	}
}