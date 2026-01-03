#pragma once

#include <string>
#include <algorithm>
#include <random>

#include "Engine.h"
#include "Presenter.h"
#include "TextField.h"
#include "Player.h"
#include "Tile.h"
#include "Camera.h"
#include "GrassBlock.h"

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

	Tile m_map[MAP_HEIGHT][MAP_WIDTH];

	Player m_player;
private:
	void updateMap();
	void drawMap();

	SDL_Texture* m_background;

	Camera m_camera;
};