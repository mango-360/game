#pragma once

#include "defines.h"
#include "Tile.h"

class GrassBlock : public Tile
{
public:
	GrassBlock();
	~GrassBlock();
	void init(float2 coords);
};