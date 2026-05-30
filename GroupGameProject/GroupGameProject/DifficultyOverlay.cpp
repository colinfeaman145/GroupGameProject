#include "DifficultyOverlay.hpp"
#include "PercentageBar.hpp"
#include "Text.hpp"
#include "Camera.hpp"
#include "GameContext.hpp"
#include <unordered_map>
#include <string>
#include "InlineHelper.hpp"

#define SECIONS_PER_DIFFICULTY 3
#define SECTION_SIZE 50 // because of percentage bar section visualisation
#define BAR_MAXIMUM SECIONS_PER_DIFFICULTY * SECTION_SIZE

std::unordered_map<DifficultyLevels, Color> difficultyColors = {
    {DifficultyLevels::BABY,      { 72, 214,  72, 255}}, // Light Green
    {DifficultyLevels::DOABLE,    {146, 224,  69, 255}}, // Cyan
    {DifficultyLevels::HARD,      {255, 215,   0, 255}}, // Gold
    {DifficultyLevels::INHUMAN,   {255, 140,   0, 255}}, // Dark Orange
    {DifficultyLevels::NIGHTMARE, {220,  20,  60, 255}}, // Crimson
    {DifficultyLevels::DEMONIC,   {137,  17,  53, 255}}, // Purple
    {DifficultyLevels::ABSURD,    { 96,  11,  55, 255}}, // Magenta
    {DifficultyLevels::HELL,      {  0,   0,   0, 255}}, // Dark Red
};


DifficultyOverlay::DifficultyOverlay(int x, int y, int w, int h, Color fillColor, Color borderColor, int alpha, int borderThickness)
 : Container(x,y,w,h,fillColor, borderColor, alpha, borderThickness){
}

DifficultyOverlay::~DifficultyOverlay() {
    delete difficultyBar;
    difficultyBar = nullptr;
}

bool DifficultyOverlay::Initialize() {

	difficultyBar = new PercentageBar(0, BAR_MAXIMUM, size.x, size.y / 2, {252, 194, 0, 255}, {150, 50, 50, 255}, RenderLayer::UI);
    difficultyBar->SetOffset(0, 0);

    currentTimeText = new Text();
    currentTimeText->Initialize("00.00", "../../assets/fonts/pixelpurl/PixelPurl.ttf", 50);
    currentTimeText->SetDrawLayer(RenderLayer::UI);
    AddSprite(currentTimeText, size.x / 2 - currentTimeText->GetWidth() / 2, difficultyBar->GetHeight());

    currentLevelText = new Text();
    currentLevelText->Initialize("Lvl 00", "../../assets/fonts/pixelpurl/PixelPurl.ttf", 50);
    currentLevelText->SetDrawLayer(RenderLayer::UI);
    AddSprite(currentLevelText, size.x / 2 - currentLevelText->GetWidth() / 2, difficultyBar->GetHeight() + currentTimeText->GetHeight());

    return true;
}

void DifficultyOverlay::Process(float deltaTime) {
    Container::Process(deltaTime);

    // update displayed time
    auto updatedTime = std::format("{:.2f}", context.timer->ElapsedSeconds());
    currentTimeText->SetText(updatedTime);

    // update difficulty
    currentLevelValue = context.dc->CurrentLevel() / SECIONS_PER_DIFFICULTY;
    auto currentDifficultyValue = currentLevelValue / SECIONS_PER_DIFFICULTY;
    auto currentBarProgress = BAR_MAXIMUM * (currentDifficultyValue - (int)currentDifficultyValue);

    // only update if the last level is not reached yet
    if ((DifficultyLevels)currentDifficultyValue <= DifficultyLevels::HELL && currentBarProgress < BAR_MAXIMUM) {
        difficultyBar->SetValues(currentBarProgress, BAR_MAXIMUM);
        difficultyBar->SetFillColor(difficultyColors[(DifficultyLevels)currentDifficultyValue]);
    }
    UpdateCurrentLevel();
}

void DifficultyOverlay::UpdateCurrentLevel() {
    if (currentLevel != (int)currentLevelValue) {
        currentLevel = (int)currentLevelValue;
        currentLevelText->SetText(std::format("Lvl {}", currentLevel));
        context.dc->EmitOnLevelUp();
    }
}

void DifficultyOverlay::Draw(Renderer* renderer) {
    Container::Draw(renderer);
	difficultyBar->SetPosition(GetPosition().x, GetPosition().y);
    difficultyBar->Draw(renderer);
}

