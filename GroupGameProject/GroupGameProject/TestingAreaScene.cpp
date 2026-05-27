#include "TestingAreaScene.hpp"
#include "Grid.hpp"
#include "Player.hpp"
#include "FlyingDevil.hpp"
#include "GameContext.hpp"
#include "AnimatedSprite.hpp"


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


	enemy = new FlyingDevil();
	enemy->Initialize(Vector2(2000, 1000));
    AddElement(enemy);

	auto enemy2 = new FlyingDevil();
	enemy2->Initialize(Vector2(2000, 2000));
    AddElement(enemy2);

	auto enemy3 = new FlyingDevil();
	enemy3->Initialize(Vector2(2000, 3000));
    AddElement(enemy3);

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
