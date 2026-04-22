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
	bool getIsSolid() { return m_isSolid; };
	SDL_Rect getTileRect() { return m_tile.rect; };
	SDL_FRect getTileGridRect() { return m_gridRect; };
	TILE_TYPE getTileType() { return m_type; };
	TYLE_DROP getTileDrop() { return m_drop; };

protected:

	int m_hp;

	float m_friction;

	bool m_isSolid;

	Drawable m_tile;

	TILE_TYPE m_type;

	TYLE_DROP m_drop;

	SDL_FRect m_gridRect;

	void updateZoom();

	bool isBroken() { return m_hp <= 0; };

	void dealDamage(int damage) { m_hp -= damage; };
private:
};