#pragma once

#include "defines.h"
#include "Tile.h"

class Bush : public Tile
{
public:
	Bush();
	~Bush();
	void init(float2 coords) override;
};