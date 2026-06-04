#include "EnemySpawner.hpp"
#include "GameContext.hpp"
#include "Enemy.hpp"
#include "GridCell.hpp"
#include "Player.hpp"
#include "EnemyId_2.hpp"
#include "EnemyId_3.hpp"

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
		if (currentEnemyCount >= maxEnemyCount) break;
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
		for (const auto& loc : bossSpawnLocations) {
			if (currentBossCount >= maxBossCount) break;
			auto bossDef = context.er->GetRandomEntityOfType(EntityType::BOSS);
			SpawnEnemy(bossDef, loc);
		}

	}
}

void EnemySpawner::SpawnEnemy(EntityDef entityDef, SpawnLocation location) {
	if (entityDef.id == 0) return;
	auto enemy = CreateEntityFromJson(entityDef.data);
	enemy->Initialize(location.cell->GetCenter() - enemy->GetRadius());

	if (auto e = dynamic_cast<Enemy*>(enemy)) {
		e->SetTarget(currentTarget);
		location.cell->AddEnemy(e);
		activeEnemies.push_back(e);
		currentEnemyCount++;
	}

}

Entity* EnemySpawner::CreateEntityFromJson(json data) {
	Entity* newEntity = nullptr;
	if (data["id"] == 1) {
		newEntity = new Player();
		if (auto entity = dynamic_cast<Entity*>(newEntity)) {
			entity->data = data;
		}
	}
	if (data["id"] == 2) {
		newEntity = new EnemyId_2();
		if (auto entity = dynamic_cast<Entity*>(newEntity)) {
			entity->data = data;
		}

	}
	if (data["id"] == 3) {
		newEntity = new EnemyId_3();
		if (auto entity = dynamic_cast<Entity*>(newEntity)) {
			entity->data = data;
		}
	}
	return newEntity;
}
