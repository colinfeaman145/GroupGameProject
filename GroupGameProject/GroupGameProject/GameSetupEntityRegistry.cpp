#include "Game.hpp"

#include <fstream>
#include "GameContext.hpp"

void Game::SetupEntityRegistry(const std::string& filepath) {

    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Failed to open entity file: " << filepath << endl;
        return;
    }
    json data = json::parse(file);

	context.er->RegisterEntity({
		.id = 1,
		.type = EntityType::PLAYER,
		.data = data["1"]
	});
	context.er->RegisterEntity({
		.id = 2,
		.type = EntityType::GENERIC,
		.data = data["2"]
	});
	context.er->RegisterEntity({
		.id = 3,
		.type = EntityType::GENERIC,
		.data = data["3"]
	});
	context.er->RegisterEntity({
		.id = 4,
		.type = EntityType::BOSS,
		.data = data["4"]
	});
}
