#include "Mob.h"

Player* Mob::m_player = nullptr;

Mob::Mob()
{
}

Mob::~Mob()
{
}

void Mob::update()
{
	zoomUpdate();

	move();

	calculateVelocity();

	collision();

	applyVelocity();

	stopOutOfBounds();
}

void Mob::move()
{
	moveVertical();

	for(auto i : normalDirs)
	{
		if (i.x != 0.0f && m_map[getIntCoords().y][getIntCoords().x - static_cast<int>(i.x)]->getTileType() != TILE_TYPE::NONE_TYPE)
		{
			jump();
			break;
		}
	}

	normalDirs.clear();
}

void Mob::moveVertical()
{
	if (m_player->getMapRect().x < hitbox.rect.x)
	{
		velocity.x = -maxInputVelocity.x * 2.0f / 5.0f;
		srcRect.x = TILE_SIZE;
	}
	else
	{
		velocity.x = maxInputVelocity.x * 2.0f / 5.0f;
		srcRect.x = 0;
	}
}
