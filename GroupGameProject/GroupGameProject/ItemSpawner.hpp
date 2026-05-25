#pragma once
#include "ItemEffect.hpp"
#include "ItemRegistry.hpp"

#include <vector>


struct SpawnElement {
	ItemDef def;
	float dropChance;
	float count;
};

class ItemSpawner  {

public:
	bool Initialise(json settings);
	void SpawnItems(Vector2 basePos);
private:
	std::vector<SpawnElement> itemsToSpawn;
	float spawnRadius;
};

