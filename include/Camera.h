#pragma once

#include "defines.h"

class Camera
{
	public:
	Camera();
	~Camera();
	void init(Player* player, Ground* ground);
	void update();
	void draw();
	int2 getPosition() { return position; };

private:
	void move();
	void updateMap();
	void drawMap();

	int2 position;
	
	float moveSpeed;

	Player* m_player;

	Ground* m_ground;
};