#pragma once

#include <string>
#include <algorithm>
#include <random>

#include "Engine.h"
#include "Presenter.h"
#include "TextField.h"
#include "Player.h"
#include "Ground.h"
#include "Tile.h"
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
private:
	SDL_Texture* m_background;

	Player m_player;

	Ground m_ground;


	Camera m_camera;
};