#pragma once

#include "defines.h"
#include "Tile.h"

class DirtBlock : public Tile
{
	public:
	DirtBlock();
	~DirtBlock();
	void init(float2 coords);
};