#include "Game.hpp"

#include <fstream>
#include "ItemRegistry.hpp"
#include "json.hpp"
#include "GameContext.hpp"

#include "Item.hpp"
#include "Gun.hpp"
#include "BisonSteak.hpp"
#include "Coin.hpp"

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
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<Gun>(data["1"]),
		.data = data["1"]
	});
	context.ir->RegisterItem({
		.id = 2,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<BisonSteak>(data["2"]),
		.data = data["2"]
	});
	context.ir->RegisterItem({
		.id = 3,
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<Coin>(data["3"]),
		.data = data["3"]
	});
}
