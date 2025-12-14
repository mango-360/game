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
	SDL_Rect getCameraRect() const { return cameraRect; };

private:
	SDL_Rect cameraRect;

	Player* m_player;
};