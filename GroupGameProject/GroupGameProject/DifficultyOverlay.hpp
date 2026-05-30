#pragma once
#include "Container.hpp"
#include <math.h>



class Timer;
class PercentageBar;
class DifficultyOverlay : public Container {
public:
	DifficultyOverlay(int x, int y, int w, int h, Color fillColor, Color borderColor, int alpha, int borderThickness);
	~DifficultyOverlay();
	bool Initialize();
	void Process(float deltaTime) override;
	void UpdateCurrentLevel();
	void Draw(Renderer* renderer) override;

private:
	Text* currentTimeText;
	Text* currentLevelText;
	PercentageBar* difficultyBar;

	float currentLevelValue;
	int currentLevel; // needed for comparison for OnLevelUp event
};
