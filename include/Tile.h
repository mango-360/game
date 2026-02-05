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
	float getFriction() { return m_friction; };
	SDL_Rect getTileRect() { return m_tile.rect; };
	SDL_FRect getTileGridRect() { return m_gridRect; };
	TILE_TYPE getTileType() { return m_type; };

protected:

	int m_hp;

	float m_friction;

	Drawable m_tile;

	TILE_TYPE m_type;

	SDL_FRect m_gridRect;

	void updateZoom();
private:
};