#pragma once

#include "defines.h"

class Camera
{
	public:
	Camera();
	~Camera();
	void init(Ground* ground);
	void update();
	void draw();
	int2 getPosition();
private:
	Ground* m_ground; // to draw ground tiles

	void move();
	void zoomUpdate();

	int2 position;
	int lastKeyPressed = -1;
	
	float zoom;
	float moveSpeed;
};