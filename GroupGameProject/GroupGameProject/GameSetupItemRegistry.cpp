#include "Game.hpp"
#include "Gun.hpp"
#include "ItemRegistry.hpp"
#include "BisonSteak.hpp"


void Game::SetupItemRegistry() {
	context.ir->RegisterItem({
		.id = 1,
		.name = "Gun",
		.description = "A basic gun that shoots bullets.",
		.tier = ItemTier::Common,
		.effect = new Gun()
	});
	context.ir->RegisterItem({
		.id = 2,
		.name = "BisonSteak",
		.description = "Get BUFFED",
		.tier = ItemTier::Common,
		.effect = new BisonSteak()
	});
}
