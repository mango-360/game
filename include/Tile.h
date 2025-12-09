#pragma once

#include "defines.h"

class Tile
{
public:

	Tile();
	~Tile();
	virtual void init();
	void update();
	void draw();
	void destroy();
	void setPosition(int2 coords);

protected:
	Drawable m_tile;

	TILE_TYPE m_type;

	int m_hp;
private:

	void updateZoom();
};