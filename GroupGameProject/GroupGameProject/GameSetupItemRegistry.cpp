#include "Game.hpp"

#include <fstream>
#include "ItemRegistry.hpp"
#include "json.hpp"
#include "GameContext.hpp"

#include "Item.hpp"
#include "Items.hpp"


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
		.tier = ItemTier::Common,
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
	context.ir->RegisterItem({
		.id = 9,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_9>(data["9"]),
		.data = data["9"]
		});
	context.ir->RegisterItem({
		.id = 10,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_10>(data["10"]),
		.data = data["10"]
		});
	context.ir->RegisterItem({
		.id = 11,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_11>(data["11"]),
		.data = data["11"]
		});
	context.ir->RegisterItem({
		.id = 12,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_12>(data["12"]),
		.data = data["12"]
		});
	context.ir->RegisterItem({
		.id = 13,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_13>(data["13"]),
		.data = data["13"]
		});
	context.ir->RegisterItem({
		.id = 14,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_14>(data["14"]),
		.data = data["14"]
		});
	context.ir->RegisterItem({
		.id = 15,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_15>(data["15"]),
		.data = data["15"]
		});
	context.ir->RegisterItem({
		.id = 16,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_16>(data["16"]),
		.data = data["16"]
		});
	context.ir->RegisterItem({
		.id = 17,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_17>(data["17"]),
		.data = data["17"]
		});
	context.ir->RegisterItem({
		.id = 18,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_18>(data["18"]),
		.data = data["18"]
		});
	context.ir->RegisterItem({
		.id = 19,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_19>(data["19"]),
		.data = data["19"]
		});
	context.ir->RegisterItem({
		.id = 20,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_20>(data["20"]),
		.data = data["20"]
		});
	context.ir->RegisterItem({
		.id = 21,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<ItemId_21>(data["21"]),
		.data = data["21"]
		});
}