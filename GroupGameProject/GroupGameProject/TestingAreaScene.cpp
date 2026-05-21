#include "TestingAreaScene.hpp"
#include "Grid.hpp"

bool TestingAreaScene::Initialize() {

    // grid setup
    context.grid = new Grid(GRID_WIDTH, GRID_HEIGHT, CELL_SIZE);
    SDL_Texture* grassTex = context.txm->LoadTexture(context.renderer, "../../assets/sprites/DungeonTextures/dungeon_floor_with_moss.png");
    context.grid->Initialize(grassTex);
    elements.push_back(context.grid);


    // player setup
    player = new Player();
    player->Initialize(Vector2(0,0), nullptr);
    elements.push_back(player);

    return true;
}

void TestingAreaScene::Process(float deltaTime) {
    for (Element* e : elements) {
        e->Process(deltaTime);
    }
}

void TestingAreaScene::Draw(Renderer* renderer) {
    for (Element* e : elements) {
        e->Draw(renderer);
    }
}

void TestingAreaScene::ReadInputs(float deltaTime) {
    //needs to be implemented 
}
