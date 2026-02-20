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
	stream >> tmp >> moveSpeed;
	stream >> tmp >> jumpStrength;

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
	zoomUpdate();

	move();

	calculateVelocity();

	collision();

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
		tmp.rect.x - (rect.w / 2 - tmp.rect.w / 2),
		tmp.rect.y - (rect.h / 2 - tmp.rect.h / 2),
		rect.w,
		rect.h
	};
	tmpEntity.texture = texture;

	drawObject(tmpEntity);
	//drawObject(tmp); //hitbox
}

int2 Entity::getRealCoords()
{
	return { static_cast<int>(hitbox.rect.x * TILE_SIZE * InputManager::getZoom()), static_cast<int>(hitbox.rect.y * TILE_SIZE * InputManager::getZoom()) };
}

void Entity::jump()
{
	if (isOnGround) velocity.y = -jumpStrength;

	isOnGround = false;
}

void Entity::zoomUpdate()
{
	rect.w = srcRect.w * InputManager::getZoom();
	rect.h = srcRect.h * InputManager::getZoom();
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

void Entity::collision()
{
	bool hitsGround = false;

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
				/*cout << "Entity at: " << floor(hitbox.rect.x) << ", " << floor(hitbox.rect.y) << endl;
				cout << "Checking block at: " << j << ", " << i << endl;
				cout << "Contact normal: " << cn << endl;*/

				collsList.push_back({ {i, j}, t });

				if (cn.y == -1)
				{
					hitsGround = true;
				}

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
		SDL_FRect tileRect = m_map[j.first.x][j.first.y]->getTileGridRect();
		ResolveDynamicRectVsRect(&hitbox.rect, velocity, &tileRect);
	}

	isOnGround = hitsGround;
}

void Entity::calculateVelocity()
{
	velocity += calculateNetForce();

	if (velocity.x != 0 && abs(velocity.x) < 0.001f) velocity.x = 0;
	if (velocity.y != 0 && abs(velocity.y) < 0.001f) velocity.y = 0;
}

float2 Entity::calculateNetForce()
{
	return gravity;
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
