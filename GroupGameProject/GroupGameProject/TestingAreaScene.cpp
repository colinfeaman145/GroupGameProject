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
    player->Initialize(Vector2(0,0), idle);
	AddElement(player);

    return true;
}

void TestingAreaScene::Process(float deltaTime) {

	// fill in elements to add from last frame
	if (!elementsToAdd.empty()) {
		for (Element* e : elementsToAdd) {
			elements.push_back(e);
		}
		elementsToAdd.clear();
	}

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
