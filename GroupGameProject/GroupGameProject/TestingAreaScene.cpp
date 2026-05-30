#include "TestingAreaScene.hpp"
#include "Grid.hpp"
#include "Player.hpp"
#include "FlyingDevil.hpp"
#include "GameContext.hpp"
#include "AnimatedSprite.hpp"
#include "ItemShopSocket.hpp"
#include "PlayerHUD.hpp"
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
    player = new Player();
    player->Initialize(Vector2(GRID_WIDTH / 2, GRID_HEIGHT / 2));
    context.grid->UpdateOccupancy((Entity*)player, &GridCell::AddOther, &GridCell::RemoveOther);
	AddElement(player);



    //player hud?
    playerHUD = new PlayerHUD(player);
    playerHUD->Initialize();
    AddElement(playerHUD);

    /*
	//animation setup
    AnimatedSprite* enemyIdle;
    SDL_Texture* enemyIdleTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Enemy/big_demon.png");
    enemyIdle = new AnimatedSprite();
    enemyIdle->Initialize(enemyIdleTexture, 34, 34, 0, 0, 500, 500, 3, 4);
    enemyIdle->SetDrawLayer(RenderLayer::ENEMIES);
    enemyIdle->SetFrameDuration(0.10);
    enemyIdle->SetLooping(true);
    enemyIdle->SetLeaveOnLastFrame(true);
    */
	//enemy = new FlyingDevil();
	//enemy->Initialize(Vector2(2000, 1000));
 //   AddElement(enemy);

 //   auto shopSocket = new ItemShopSocket();
 //   shopSocket->Initialize(Vector2(3000, 1000), 2);
 //   AddElement(shopSocket);

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
