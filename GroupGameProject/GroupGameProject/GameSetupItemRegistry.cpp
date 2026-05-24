#include "Game.hpp"
#include "Gun.hpp"
#include "ItemRegistry.hpp"
#include "BisonSteak.hpp"
#include <fstream>
#include "json.hpp"

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
		.name = data["1"]["name"].get<std::string>(),
		.description = data["1"]["description"].get<std::string>(),
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<Gun>(data["1"])
	});
	context.ir->RegisterItem({
		.id = 2,
		.name = data["2"]["name"].get<std::string>(),
		.description = data["2"]["description"].get<std::string>(),
		.tier = ItemTier::Common,
		.effect = ItemEffect::CreateItemEffectFromJson<BisonSteak>(data["2"])
	});
}
