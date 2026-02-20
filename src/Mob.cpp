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
		if (i.x != 0.0f) 
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
	velocity.x = moveSpeed * 4 / 5;
}
