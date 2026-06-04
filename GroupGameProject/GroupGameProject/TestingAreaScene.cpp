#include "TestingAreaScene.hpp"
#include "GameContext.hpp"
#include "Player.hpp"
#include "DungeonGenerator.hpp"
#include "Camera.hpp"
#include "EnemySpawner.hpp"

TestingAreaScene::~TestingAreaScene() {
    delete player;
    player = nullptr;
    delete spawner;
    spawner = nullptr;
}

bool TestingAreaScene::Initialize() {
    spawner = new EnemySpawner();
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

	spawner->Process(deltaTime);
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
	auto playerEntity = EnemySpawner::CreateEntityFromJson(context.er->Get(1).data);
	if (auto p = dynamic_cast<Player*>(playerEntity)) {
		player = p;
	    player->Initialize({ 0,0 });
	    GenerateNewMap(player);
	}

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

	spawner->Initialise(dg->GetEnemySpawnLocations(), 5, 1, player);
}

void TestingAreaScene::ReadInputs(float deltaTime) {
    // reload map layout
    if (context.im->IsKeyDown("ctrl") && context.im->IsKeyDown("shift") && context.im->IsKeyDown("r")) {
        ResetGameState();
    }
}
