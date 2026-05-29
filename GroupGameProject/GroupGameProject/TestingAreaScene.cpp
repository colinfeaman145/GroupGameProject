#include "TestingAreaScene.hpp"
#include "Grid.hpp"
#include "Player.hpp"
#include "FlyingDevil.hpp"
#include "GameContext.hpp"
#include "AnimatedSprite.hpp"
#include "ItemShopSocket.hpp"
#include "PlayerHUD.hpp"

bool TestingAreaScene::Initialize() {

    // grid setup
    context.grid = new Grid(GRID_WIDTH, GRID_HEIGHT, CELL_SIZE);
    SDL_Texture* grassTex = context.txm->LoadTexture(context.renderer, "../../assets/sprites/DungeonTextures/dungeon_floor_with_moss.png");
    context.grid->Initialize(grassTex);
    AddElement(context.grid);


    // player setup
    player = new Player();
    player->Initialize(Vector2(1000,1000));
    context.grid->UpdateOccupancy((Entity*)player, &GridCell::AddOther, &GridCell::RemoveOther);
	AddElement(player);



    //player hud?
    playerHUD = new PlayerHUD(player);
    playerHUD->Initialize();
    AddElement(playerHUD);


	enemy = new FlyingDevil();
	enemy->Initialize(Vector2(2000, 1000));
    enemy->SetTarget(player);
    AddElement(enemy);

	auto enemy1 = new FlyingDevil();
	enemy1->Initialize(Vector2(2000, 2000));
    enemy1->SetTarget(player);
    AddElement(enemy1);

	auto enemy2 = new FlyingDevil();
	enemy2->Initialize(Vector2(2000, 3000));
    enemy2->SetTarget(player);
    AddElement(enemy2);


    auto shopSocket1 = new ItemShopSocket();
    shopSocket1->Initialize(Vector2(3000, 1000), 2);
    AddElement(shopSocket1);

    auto shopSocket2 = new ItemShopSocket();
    shopSocket2->Initialize(Vector2(3500, 1000), 6);
    AddElement(shopSocket2);

    auto shopSocket3 = new ItemShopSocket();
    shopSocket3->Initialize(Vector2(4000, 1000), 3);
    AddElement(shopSocket3);

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
