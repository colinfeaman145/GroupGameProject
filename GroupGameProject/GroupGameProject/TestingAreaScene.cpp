#include "TestingAreaScene.hpp"
#include "Grid.hpp"

bool TestingAreaScene::Initialize() {

    // grid setup
    context.grid = new Grid(GRID_WIDTH, GRID_HEIGHT, CELL_SIZE);
    SDL_Texture* grassTex = context.txm->LoadTexture(context.renderer, "../../assets/sprites/DungeonTextures/dungeon_floor_with_moss.png");
    context.grid->Initialize(grassTex);
    AddElement(context.grid);


	//animation setup
    AnimatedSprite* idle;
    SDL_Texture* playerIdle = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Soldier/soldier_idle.png");
    idle = new AnimatedSprite();
    idle->Initialize(playerIdle, 34, 34, 0, 0, 500, 500, 3, 5);
    idle->SetDrawLayer(RenderLayer::PLAYER);
    idle->SetFrameDuration(0.25);
    idle->SetLooping(true);
    idle->SetLeaveOnLastFrame(true);

    // player setup
    player = new Player();
    player->Initialize(Vector2(1000,1000), idle);
    context.grid->UpdateOccupancy((Entity*)player, &GridCell::AddOther, &GridCell::RemoveOther);
	AddElement(player);


	//animation setup
    AnimatedSprite* enemyIdle;
    SDL_Texture* enemyIdleTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Enemy/big_demon.png");
    enemyIdle = new AnimatedSprite();
    enemyIdle->Initialize(enemyIdleTexture, 34, 34, 0, 0, 500, 500, 3, 4);
    enemyIdle->SetDrawLayer(RenderLayer::ENEMIES);
    enemyIdle->SetFrameDuration(0.10);
    enemyIdle->SetLooping(true);
    enemyIdle->SetLeaveOnLastFrame(true);

	enemy = new Enemy();
	enemy->Initialize(Vector2(2000, 1000), enemyIdle, 0, 0, 0, 0);
    context.grid->UpdateEnemyOccupancy(enemy);
	AddElement(enemy);

    return true;
}

void TestingAreaScene::Process(float deltaTime) {
	Scene::Process(deltaTime);

    //collision updates
    context.grid->UpdateOccupancy((Entity*)player, &GridCell::AddOther, &GridCell::RemoveOther);
    context.grid->ResolveCollisions(player); //collison updates

}

void TestingAreaScene::Draw(Renderer* renderer) {
	Scene::Draw(renderer);
}

void TestingAreaScene::ReadInputs(float deltaTime) {
    //needs to be implemented 
}
