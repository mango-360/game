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
	void destroy();

	void reset();
	
	int getStackSize() const { return stackSize; }
	DROP_TYPE getDropType() const { return m_dropType; }

	SDL_FRect getGridRect() const { return m_gridRect; }

	DrawableWithOpacity m_dropDrawable;
protected:
	void zoomUpdate();

	int stackSize = 5;
	

	DROP_TYPE m_dropType;
	TILE_TYPE m_dropTileType;

	SDL_FRect m_gridRect;
};