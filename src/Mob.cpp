#include "Mob.h"

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
			cout << i << endl;

			jump();
			break;
		}
	}

	normalDirs.clear();
}

void Mob::moveVertical()
{
	velocity.x = maxInputVelocity.x * 2.0f / 5.0f;
}
