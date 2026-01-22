#pragma once

#include "defines.h"

class Tile
{
public:

	Tile();
	~Tile();
	virtual void init(float2 coords);
	void update();
	//void draw();                 // legacy draw (no camera) - kept for compatibility
	void draw(float2 camCoords); // draw with camera offset
	void destroy();
	SDL_Rect getTileRect() { return m_tile.rect; };
	SDL_FRect getTileGridRect() { return m_gridRect; };

protected:
	Drawable m_tile;

	TILE_TYPE m_type;

	int m_hp;

	SDL_FRect m_gridRect;

	void updateZoom();
private:
};