#pragma once

#include <string>
#include <algorithm>
#include <random>
#include <cmath>

#include "Engine.h"
#include "Presenter.h"
#include "TextField.h"
#include "DialogPopUp.h"

#include "Camera.h"
#include "Statistics.h"

#include "Player.h"
#include "Mob.h"

#include "Projectile.h"

#include "Tile.h"
#include "GrassBlock.h"
#include "DirtBlock.h"
#include "Bush.h"

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
	Mob m_mob;

	vector<Entity*> m_entities;

	vector<unique_ptr<Projectile>> m_projectiles;

private:
	void updateMap();
	void drawMap();
	void toggleStatistics();
	void destroyProjectiles();

	void handleCollisions();
	void handleEntityTileCollisions();
	void handleEntityEntityCollisions();
	void handleEntityProjectileCollisions();
	void handleProjectileTileCollisions();

	bool drawStatistics = false;

	SDL_Texture* m_background;

	Camera m_camera;

	Statistics m_statistics;

	DialogPopUp m_dialog;
};