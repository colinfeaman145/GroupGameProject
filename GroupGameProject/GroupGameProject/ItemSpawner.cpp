#include "ItemSpawner.hpp"
#include "GameContext.hpp"
#include "ItemRegistry.hpp"
#include "Item.hpp"
#include "InlineHelper.hpp"


bool ItemSpawner::Initialise(json config) {

	if (config["items"].empty()) return false;

	spawnRadius = config["spawnRadius"];
	for (auto itemId: config["items"]) {
		auto item = context.ir->Get(itemId["id"]);
		auto dropChance = itemId["dropRatePerItem"];
		auto count = itemId["count"];
		
		itemsToSpawn.push_back({item, dropChance, count});
	}

	return true;
}
void ItemSpawner::SpawnItems(Vector2 basePos) {
	for (auto item : itemsToSpawn) {
		for (int i = 0; i < item.count; i++) {

			if (!HasHitChance(item.dropChance)) continue;
		
			auto newItem = new Item(item.def.data);

			// generate random pos in a radius
			std::uniform_real_distribution<float> dis(0.0, 1.0);
			float theta = 2.0 * PI * dis(gen);
			float radius = spawnRadius * std::sqrt(dis(gen));

			Vector2 randomPosInRadius = { basePos.x + radius * std::cos(theta), basePos.y + radius * std::sin(theta) };
			newItem->SetBasePos(randomPosInRadius);
			context.currentScene->AddElement(newItem);
		}
	}
}




