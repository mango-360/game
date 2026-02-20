#pragma once

#include "defines.h"
#include "Entity.h"

class Mob : public Entity
{
public:
	Mob();
	~Mob();
	void update() override;

private:
	void move() override;
	void moveVertical() override;
};