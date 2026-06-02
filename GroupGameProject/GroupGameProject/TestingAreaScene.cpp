#include "TestingAreaScene.hpp"
#include "GameContext.hpp"
#include "Player.hpp"
#include "DungeonGenerator.hpp"
#include "Camera.hpp"

bool TestingAreaScene::Initialize() {
    context.dc->RegisterOnStageCompleted([this] {
		this->hasStageCompleted = true;
 });
    ResetGameState();
    return true;
}

void TestingAreaScene::Process(float deltaTime) {
	Scene::Process(deltaTime);
    ReadInputs(deltaTime);

	if (hasStageCompleted) {
		GenerateNewMap(player);
		hasStageCompleted = false;
	}
}

void TestingAreaScene::Draw(Renderer* renderer) {
	Scene::Draw(renderer);
	renderer->cam->Follow(player->GetPosition());
}

void TestingAreaScene::ResetGameState() {
    if (player != nullptr) {
        delete player;
        player = nullptr;
	}
	player = Entity::CreateEntityFromJson<Player>(context.er->Get(1).data);
	player->Initialize({ 0,0 });
	GenerateNewMap(player);

    context.timer->Reset();
}


void TestingAreaScene::GenerateNewMap(Player* player) {
    if (context.grid != nullptr) {
        delete context.grid;
        context.grid = nullptr;
    }
    // grid setup
    context.grid = new Grid(GRID_WIDTH, GRID_HEIGHT, CELL_SIZE);
    context.grid->Initialize();
    AddElement(context.grid);

    //make dungeon
    DungeonGenerator* dg = new DungeonGenerator(player);
    dg->LoadRooms("../../data/dungeonRooms/");
    dg->Generate();
}

void TestingAreaScene::ReadInputs(float deltaTime) {
    // reload map layout
    if (context.im->IsKeyDown("ctrl") && context.im->IsKeyDown("shift") && context.im->IsKeyDown("r")) {
        ResetGameState();
    }
}
