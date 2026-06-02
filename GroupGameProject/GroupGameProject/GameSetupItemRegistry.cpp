#include "Game.hpp"

#include <fstream>
#include "ItemRegistry.hpp"
#include "json.hpp"
#include "GameContext.hpp"

#include "Item.hpp"
#include "ItemId_1.hpp"
#include "ItemId_2.hpp"
#include "ItemId_3.hpp"
#include "ItemId_4.hpp"
#include "ItemId_5.hpp"
#include "ItemId_6.hpp"
#include "ItemId_7.hpp"
#include "ItemId_8.hpp"

using json = nlohmann::json;

void Game::SetupItemRegistry(const std::string& filepath) {

    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Failed to open items file: " << filepath << endl;
        return;
    }
    json data = json::parse(file);

	context.ir->RegisterItem({
		.id = 1,
		.tier = ItemTier::Module,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_1>(data["1"]),
		.data = data["1"]
	});
	context.ir->RegisterItem({
		.id = 2,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_2>(data["2"]),
		.data = data["2"]
	});
	context.ir->RegisterItem({
		.id = 3,
		.tier = ItemTier::Utility,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_3>(data["3"]),
		.data = data["3"]
	});
	context.ir->RegisterItem({
		.id = 4,
		.tier = ItemTier::Module,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_4>(data["4"]),
		.data = data["4"]
	});
	context.ir->RegisterItem({
		.id = 5,
		.tier = ItemTier::Module,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_5>(data["5"]),
		.data = data["5"]
	});
	context.ir->RegisterItem({
		.id = 6,
		.tier = ItemTier::Rare,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_6>(data["6"]),
		.data = data["6"]
	});
	context.ir->RegisterItem({
		.id = 7,
		.tier = ItemTier::Module,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_7>(data["7"]),
		.data = data["7"]
	});
	context.ir->RegisterItem({
		.id = 8,
		.tier = ItemTier::Utility,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_8>(data["8"]),
		.data = data["8"]
	});

}
