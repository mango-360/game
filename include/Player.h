#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	~Player();
	void init();
	void update();
private:
	
	int lastKeyPressed = -1;
};