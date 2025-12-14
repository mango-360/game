#pragma once

#include "defines.h"

class Tile
{
public:

	Tile();
	~Tile();
	virtual void init(int2 coords);
	void update();
	//void draw();                 // legacy draw (no camera) - kept for compatibility
	void draw(int2 camOffset); // draw with camera offset
	void destroy();
	void setPosition(int2 coords);

protected:
	Drawable m_tile;

	TILE_TYPE m_type;

	int m_hp;

	int2 m_gridCoords;
private:

	void updateZoom();
};