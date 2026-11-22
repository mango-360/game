#pragma once

#include "defines.h"
#include "Tile.h"
#include "Drop.h"

class LootTable
{
public:

	LootTable();
	~LootTable();

	static const vector<pair<DROP_TYPE, int>> generateLoot(TILE_TYPE tileType);
private:
	static const vector<pair<DROP_TYPE, int>> independentGen(const vector<tuple<DROP_TYPE, int, float>>& lootPool);
};