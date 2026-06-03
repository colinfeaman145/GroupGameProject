#include "EnemySpawner.hpp"
#include "GameContext.hpp"
#include "Enemy.hpp"
#include "GridCell.hpp"
#include "Player.hpp"

EnemySpawner::EnemySpawner() :
	maxEnemyCount(0),
	currentEnemyCount(0),
	maxBossCount(0),
	currentBossCount(0) {}

bool EnemySpawner::Initialise(vector<SpawnLocation> locations, int maxEnemyCount, int maxBossCount, Entity* target) {
	this->maxEnemyCount = maxEnemyCount;
	this->maxBossCount = maxBossCount;
	this->currentTarget = target;

	for (const auto& loc : locations) {
		if (loc.type == EntityType::BOSS) {
			bossSpawnLocations.push_back(loc);
		}
		else if (loc.type == EntityType::GENERIC){
			enemySpawnLocations.push_back(loc);
		}
	}

	SpawnInBossesUntilMax();
	SpawnInEnemiesUntilMax();

	return true;
}

void EnemySpawner::Process(float deltaTime) {

	std::erase_if(activeEnemies, [](Enemy* e) { return !e->IsAlive();});
	currentEnemyCount = activeEnemies.size();
	if (currentEnemyCount >= maxEnemyCount) return;

	SpawnInEnemiesUntilMax();
}

void EnemySpawner::SpawnInEnemiesUntilMax() {
	// setup generic enemies
	for (int i = 0; i < maxEnemyCount; i++) {
		auto enemyDef = context.er->GetRandomEntityOfType(EntityType::GENERIC);
		auto randomLocation = enemySpawnLocations[rand() % enemySpawnLocations.size()];
		SpawnEnemy(enemyDef, randomLocation);
	}
}
void EnemySpawner::SpawnInBossesUntilMax() {
	if (bossSpawnLocations.size() > maxBossCount) {
		printf("EnemySpawner: more boss spawn locations than max boss count, some boss spawn locations will not be used\n");
	}
	else {
		// setup bosses
		for (const auto& loc : bossSpawnLocations) {
			auto bossDef = context.er->GetRandomEntityOfType(EntityType::BOSS);
			SpawnEnemy(bossDef, loc);
		}

	}
}

void EnemySpawner::SpawnEnemy(EntityDef entityDef, SpawnLocation location) {
	if (entityDef.id == 0) return;
	auto enemy = Enemy::CreateEntityFromJson<Enemy>(entityDef.data);
	enemy->Initialize(location.cell->GetCenter() - enemy->GetRadius());
	enemy->SetTarget(currentTarget);
	location.cell->AddEnemy(enemy);

	location.cell->AddOther(enemy);
	activeEnemies.push_back(enemy);
	currentEnemyCount++;
}
