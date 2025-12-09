#pragma once

#include "defines.h"

class Camera
{
	public:
	Camera();
	~Camera();
	void init(Player* player);
	void update();
	void draw();
	float2 getPosition() { return position; };

private:
	void move();
	void zoom();
	void updateTileSize();
	void drawMap();

	float2 position;
	
	float moveSpeed;

	Player* m_player;

	float prevZoom = 1.0f;
	float prevTileSize;
};