#pragma once
#include "EntityRegistry.hpp"
#include <vector>
class GridCell;
class Entity;
class Enemy;

struct SpawnLocation {
	GridCell* cell;
	EntityType type;
};

class EnemySpawner {
public:
	EnemySpawner();
	bool Initialise(std::vector<SpawnLocation> locations, int maxEnemyCount, int maxBossCount, Entity* target);
	void Process(float deltaTime);
	void SpawnEnemy(EntityDef enemy, SpawnLocation location);
	void SpawnInEnemiesUntilMax();
	void SpawnInBossesUntilMax();

private:
	Entity* currentTarget;
	int maxEnemyCount;
	int currentEnemyCount;

	int maxBossCount;
	int currentBossCount;
	std::vector<SpawnLocation> enemySpawnLocations;
	std::vector<SpawnLocation> bossSpawnLocations;
	std::vector<Enemy*> activeEnemies;
};

