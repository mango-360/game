#pragma once

#include "defines.h"

class Drop
{
public:
	Drop();
	~Drop();
	void init(int2 coords, DROP_TYPE dropType);
	void update();
	void draw(float2 camCoords);

protected:
	void zoomUpdate();

	Drawable m_dropDrawable;

	DROP_TYPE m_dropType;
	TILE_TYPE m_dropTileType;

	SDL_FRect m_gridRect;
};