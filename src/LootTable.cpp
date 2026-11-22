#include "LootTable.h"

LootTable::LootTable()
{
}

LootTable::~LootTable()
{
}

const vector<pair<DROP_TYPE, int>> LootTable::generateLoot(TILE_TYPE tileType)
{
	switch (tileType)
	{
	case TILE_TYPE::BUSH:
		return independentGen(
		{
			{DROP_TYPE::STICK, 1, 80.0f},
			{DROP_TYPE::LEAF, 1, 50.0f}
		});
	default:
		cout << "ERROR: unknown loot of tile type" << endl;
		return { {DROP_TYPE::NONE_DROP, -1} };
	}
}

const vector<pair<DROP_TYPE, int>> LootTable::independentGen(const vector<tuple<DROP_TYPE, int, float>>& lootPool)
{
	vector<pair<DROP_TYPE, int>> loot;

	for (const auto& [dropType, amount, percent] : lootPool)
		if (chance(rng) < percent)
			loot.emplace_back(dropType, amount);

	return loot;	
}
