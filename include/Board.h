#pragma once

#include <string>
#include <algorithm>
#include <random>

#include "Engine.h"
#include "Presenter.h"
#include "TextField.h"
#include "Player.h"
#include "Ground.h"
#include "Camera.h"

class Board
{
public:
	Board();
	~Board();

	void init();
	void initMap();
	void update();
	void draw();
	void destroy();

	Tile m_map[24][32];

	Ground m_ground;

	Player m_player;
private:
	SDL_Texture* m_background;

	Camera m_camera;
};