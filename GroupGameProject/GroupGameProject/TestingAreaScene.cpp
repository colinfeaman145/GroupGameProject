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
    context.am->AddGroup("SFX");
    context.am->AddGroup("Music");
    context.am->LoadSound("../../assets/audio/hit.wav", "hit", 0.1f);
    context.am->LoadSound("../../assets/audio/death.wav", "death", 0.5f);
    context.am->LoadSound("../../assets/audio/shoot_1.wav", "shoot_1", 0.05f);
    context.am->LoadSound("../../assets/audio/shoot_2.wav", "shoot_2", 0.05f);
    context.am->LoadSound("../../assets/audio/shoot_3.wav", "shoot_3", 0.05f);
    context.am->LoadSound("../../assets/audio/coin_pickup.wav", "coin_pickup", 0.2f);
    context.am->LoadSound("../../assets/audio/dash.wav", "dash", 0.3f);
    context.am->LoadSound("../../assets/audio/explosion.wav", "explosion", 0.2f);
    context.am->LoadSound("../../assets/audio/powerup.wav", "powerup", 0.3f);
    context.am->LoadSound("../../assets/audio/walking.wav", "walking", 1.5f);
    context.am->LoadMusicTrack("../../assets/audio/game_music.wav", "game_music");
    context.am->SetGroupVolume("SFX", 0.8f);
    context.am->AddGroup("Footsteps");
    context.am->SetGroupVolume("Footsteps", 0.15f);
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
    elements.clear();
    elementsToAdd.clear();
    elementsToDelete.clear();

    if (context.grid != nullptr) {
        context.grid->ClearAllEntities();
        delete context.grid;
        context.grid = nullptr;
    }

    context.grid = new Grid(GRID_WIDTH, GRID_HEIGHT, CELL_SIZE);
    context.grid->Initialize();
    AddElement(context.grid);

    DungeonGenerator* dg = new DungeonGenerator(player);
    dg->LoadRooms("../../data/dungeonRooms/");
    dg->Generate();

	spawner->Initialise(dg->GetEnemySpawnLocations(), 1, 1, player);
}

void TestingAreaScene::ReadInputs(float deltaTime) {
    // reload map layout
    if (context.im->IsKeyDown("ctrl") && context.im->IsKeyDown("shift") && context.im->IsKeyDown("r")) {
        ResetGameState();
    }
}
Player* TestingAreaScene::GetPlayer() const //To get the same player, so that the player tracking is the same
{
    return player;
}
