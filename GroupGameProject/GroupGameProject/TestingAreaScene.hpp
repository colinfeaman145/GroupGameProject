#pragma once
#include "Scene.hpp"


class Player;
class EnemySpawner;

#define GRID_WIDTH 50000
#define GRID_HEIGHT 50000
#define CELL_SIZE 500

class TestingAreaScene : public Scene {
public:
    TestingAreaScene() = default;
	~TestingAreaScene();

    bool Initialize() override;
    void Process(float deltaTime) override;
    void Draw(Renderer* renderer) override;
    void ResetGameState();
    void GenerateNewMap(Player* player);
	void ReadInputs(float deltaTime);

protected:
    Player* player;
    EnemySpawner* spawner;
    bool hasStageCompleted;
};

