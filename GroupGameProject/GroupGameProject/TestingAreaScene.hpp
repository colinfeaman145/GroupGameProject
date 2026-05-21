#pragma once
#include "Scene.hpp"
#include "Player.hpp"


#define GRID_WIDTH 15000
#define GRID_HEIGHT 15000
#define CELL_SIZE 500

class TestingAreaScene : public Scene {
public:
    TestingAreaScene() {}
    ~TestingAreaScene() {}

    bool Initialize() override;
    void Process(float deltaTime) override;
    void Draw(Renderer* renderer) override;
	void ReadInputs(float deltaTime) override;

protected:
    vector<Element*> elements;
    vector<Sprite*> UI;

    Sprite* levelBackground;
	Player* player;
};

