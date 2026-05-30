#include "DifficultyCalculator.hpp"
#include <math.h>
#include "GameContext.hpp"
#include "InlineHelper.hpp"


float DifficultyCalculator::TimeFactor() {
    return 10.1506 * pow(1, 0.2);
}

float DifficultyCalculator::StageFactor() {
    return pow(1.15, stagesCompleted);
}

float DifficultyCalculator::DifficultyCoefficient() {
    auto elapsedMinutes = context.timer->ElapsedSeconds() / 60;
    return (1 + elapsedMinutes * TimeFactor()) * StageFactor();
}

float DifficultyCalculator::CurrentLevel() {
    // cap out the level at 99
    return clip(1 + (DifficultyCoefficient() - 1) / 0.33, 0,99);
}
int DifficultyCalculator::GetStagesCompleted() {
    return stagesCompleted;
}


void DifficultyCalculator::RegisterOnLevelUp(std::function<void()> func) {
	onLevelUpCallbacks.push_back(func);
}

void DifficultyCalculator::RegisterOnStageCompleted(std::function<void()> func) {
	onStageCompletedCallbacks.push_back(func);
}
void DifficultyCalculator::EmitOnLevelUp() {
	for (auto callback : onLevelUpCallbacks) {
		callback();
	}
}

void DifficultyCalculator::EmitOnStageCompleted() {
	for (auto callback : onStageCompletedCallbacks) {
		callback();
	}
}

void DifficultyCalculator::StageCompleted() {
	EmitOnStageCompleted();
	stagesCompleted++;
}
