#pragma once
#include "Scene.hpp"

class Player;
class InGameUIOverlay : public Scene {
public:
	bool Initialize(Player*);
	void Process(float deltaTime) override;
	void Draw(Renderer* renderer) override;
	void ReadInputs(float deltaTime) override;

private:
	Player* player;
};

