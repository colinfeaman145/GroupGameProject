#pragma once
#include <functional>
#include <vector>

enum DifficultyLevels {
	BABY,
	DOABLE,
	HARD,
	INHUMAN,
	NIGHTMARE,
	DEMONIC,
	ABSURD,
	HELL
};

class DifficultyCalculator {
public:
	float TimeFactor();
	float StageFactor();
	float DifficultyCoefficient();
	float CurrentLevel();


	void RegisterOnStageCompleted(std::function<void()> func);
	void RegisterOnLevelUp(std::function<void()> func);
	void EmitOnStageCompleted();
	void StageCompleted();


	int GetStagesCompleted();

	void EmitOnLevelUp();

private:
	int stagesCompleted;
	DifficultyLevels currentDifficulty;
	std::vector<std::function<void()>> onStageCompletedCallbacks;
	std::vector<std::function<void()>> onLevelUpCallbacks;
};

