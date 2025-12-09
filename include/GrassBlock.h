#pragma once

#include "defines.h"
#include "Tile.h"

class GrassBlock : public Tile
{
public:
	GrassBlock();
	~GrassBlock();
	void init(int2 coords);
};