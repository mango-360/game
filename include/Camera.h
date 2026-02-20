#pragma once

#include "defines.h"
#include "Player.h"

class Camera
{
	public:
	Camera();
	~Camera();
	void init(Player* player);
	void update();
	Camera_Rect getCameraRect() const { return rect; };

private:
	void updateZoom();
	void smoothFollow();
	void stopOutOfBounds();

	Camera_Rect rect;

	Player* m_player;

	float prevZoom;
};