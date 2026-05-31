#include "TestingAreaScene.hpp"
#include "Grid.hpp"
#include "GameContext.hpp"
#include "AnimatedSprite.hpp"
#include "ItemShopSocket.hpp"
#include "PlayerHUD.hpp"
#include "Player.hpp"
#include "EnemyId_2.hpp"
#include "EnemyId_3.hpp"
#include "DungeonGenerator.hpp"
bool TestingAreaScene::Initialize() {

    // grid setup
    context.grid = new Grid(GRID_WIDTH, GRID_HEIGHT, CELL_SIZE);
    context.grid->Initialize();
    AddElement(context.grid);

    //make dungeon
    DungeonGenerator* dg = new DungeonGenerator();
    dg->LoadRooms("../../data/dungeonRooms/");
    dg->Generate();


    // player setup
    auto player = Entity::CreateEntityFromJson<Player>(context.er->Get(1).data);
    player->Initialize(Vector2(GRID_WIDTH / 2, GRID_HEIGHT  / 2));
    //context.grid->UpdateOccupancy((Entity*)player, &GridCell::AddOther, &GridCell::RemoveOther);
	//AddElement(player);

    //player hud?
    playerHUD = new PlayerHUD(player);
    playerHUD->Initialize();
    AddElement(playerHUD);

    context.timer->Reset();
    return true;
}

void TestingAreaScene::Process(float deltaTime) {
	Scene::Process(deltaTime);



}

void TestingAreaScene::Draw(Renderer* renderer) {
	Scene::Draw(renderer);
}

void TestingAreaScene::ReadInputs(float deltaTime) {
    //needs to be implemented 
}
