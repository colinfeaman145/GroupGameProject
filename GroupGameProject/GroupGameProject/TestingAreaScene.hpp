#pragma once
#include "Scene.hpp"

class Player;
class Enemy;
class PlayerHUD;

#define GRID_WIDTH 100000
#define GRID_HEIGHT 100000
#define CELL_SIZE 500

class TestingAreaScene : public Scene {
public:
    TestingAreaScene() {}

    bool Initialize() override;
    void Process(float deltaTime) override;
    void Draw(Renderer* renderer) override;
	void ReadInputs(float deltaTime) override;

protected:
    Sprite* levelBackground;
	Player* player;
	Enemy* enemy;
    PlayerHUD* playerHUD;
};

