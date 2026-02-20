#pragma once

#include "Entity.h"
#include "Tile.h"

class Player : public Entity
{
public:
	Player();
	~Player();
	void init(Tile(*map)[MAP_WIDTH]);
	void update();

private:
	void move();
	void moveVertical();

	void collision();
	void calculateFriction(int2 coords);
	void calculateVelocity();
	void addFriction();

	int lastKeyPressed = -1;
	float2 friction;
	float2 inputVelocity = { 0, 0 };
	float2 maxInputVelocity;
};