#include "Projectile.h"
#include "InputManager.h"
#include "Presenter.h"

vector<Mob>* Projectile::m_mobs = nullptr;

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

	calculateVelocity();

	collision();

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

void Projectile::collision()
{
	float2 cp, cn;
	float t = 0, min_t = INFINITY;
	vector<pair<int2, float>> collsList;

	// Work out collision point, add it to vector along with rect ID
	for (int i = floor(hitbox.rect.y) - 1; i <= ceil(hitbox.rect.y + hitbox.rect.h); ++i)
	{
		for (int j = floor(hitbox.rect.x) - 1; j <= ceil(hitbox.rect.x + hitbox.rect.w); ++j)
		{
			if (m_owner->m_map[i][j]->getTileType() == TILE_TYPE::NONE_TYPE) continue;

			if (DynamicRectVsRect(&hitbox.rect, velocity, m_owner->m_map[i][j]->getTileGridRect(), cp, cn, t))
			{
				/*cout << "Entity at: " << floor(hitbox.rect.x) << ", " << floor(hitbox.rect.y) << endl;
				cout << "Checking block at: " << j << ", " << i << endl;
				cout << "Contact normal: " << cn << endl;*/

				collsList.push_back({ {i, j}, t });

				normalDirs.push_back(cn);
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
		SDL_FRect tileRect = m_owner->m_map[j.first.x][j.first.y]->getTileGridRect();
		ResolveDynamicRectVsRect(&hitbox.rect, velocity, &tileRect);
	}

	for (auto j : collsList)
	{
		isAlive = false;

		break;
	}

	for (Mob& mob : *m_mobs)
	{
		if (DynamicRectVsRect(&hitbox.rect, velocity, mob.getMapRect(), cp, cn, t))
		{
			//mob.health -= damage;
			isAlive = false;
			break;
		}
	}
}

void Projectile::calculateVelocity()
{
	velocity += calculateNetForce();

	if (velocity.x != 0 && abs(velocity.x) < 0.001f) velocity.x = 0;
	if (velocity.y != 0 && abs(velocity.y) < 0.001f) velocity.y = 0;
}

float2 Projectile::calculateNetForce()
{
	return GRAVITY;
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