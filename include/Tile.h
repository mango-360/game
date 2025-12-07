#pragma once

#include "defines.h"

class Tile
{
public:

	Tile();
	~Tile();
	void init();
	void update();
	void draw();
	void destroy();

protected:
	Drawable m_tile;

	TILE_TYPE m_type;

	int m_hp;
private:

	void updateZoom();
};