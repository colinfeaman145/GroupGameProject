#include "GameOverScene.hpp"
#include "GameContext.hpp"
#include "Game.hpp"
#include "Sprite.hpp"
#include "Camera.hpp"

GameOverScene::GameOverScene()
    : titleText(nullptr)
    , timeText(nullptr)
    , stageText(nullptr)
    , itemsText(nullptr)
    , coinsText(nullptr)
    , restartButton(nullptr)
    , quitButton(nullptr)
    , minutes(0)
    , seconds(0)
    , stagesCompleted(0)
    , itemsCollected(0)
    , coinsLeft(0)
    , restartPressed(false)
    , quitPressed(false)
{
}

GameOverScene::~GameOverScene() 
{
    delete titleText;
    delete timeText;
    delete stageText;
    delete itemsText;
    delete coinsText;
    delete restartButton;
    delete quitButton;
    minutes = 0;
    seconds = 0;
    stagesCompleted = 0;
    itemsCollected = 0;
    coinsLeft = 0;
    restartPressed = false;
    quitPressed = false;
}

bool GameOverScene::Initialize() 
{
    const string fontPath = "../../assets/fonts/pixelpurl/PixelPurl.ttf";

    titleText = new Text();
    titleText->Initialize("GAME OVER", fontPath, 185);
    titleText->SetPosition((WIDTH - titleText->GetWidth()) / 2, 80);
    UI.push_back(titleText);

    timeText = new Text();
    timeText->Initialize("Time Lived: 0m 0s", fontPath, 28);
    timeText->SetPosition((WIDTH - timeText->GetWidth()) / 2, 290);
    UI.push_back(timeText);

    stageText = new Text();
    stageText->Initialize("Stages Completed: 0", fontPath, 28);
    stageText->SetPosition((WIDTH - timeText->GetWidth()) / 2, 340);//The text position is based on the timeText so that they all line up with one another
    UI.push_back(stageText);

    itemsText = new Text();
    itemsText->Initialize("Items Collected: 0", fontPath, 28);
    itemsText->SetPosition((WIDTH - timeText->GetWidth()) / 2, 390);
    UI.push_back(itemsText);

    coinsText = new Text();
    coinsText->Initialize("Coins Left After Death: 0", fontPath, 28);
    coinsText->SetPosition((WIDTH - timeText->GetWidth()) / 2, 440);
    UI.push_back(coinsText);

    restartButton = new Button(440, 500, 220, 70, { 40, 40, 40, 255 }, { 80, 80, 80, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, 4, [this]() { restartPressed = true; }, 1.05f);
    restartButton->SetPosition((WIDTH - timeText->GetWidth()) / 2, 520);
    UI.push_back(restartButton);

    quitButton = new Button(440, 590, 220, 70, { 40, 40, 40, 255 }, { 80, 80, 80, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, 4, [this]() { quitPressed = true; }, 1.05f);
    quitButton->SetPosition((WIDTH - timeText->GetWidth()) / 2, 610);
    UI.push_back(quitButton);

    Text* restartLabel = new Text();
    restartLabel->Initialize("RESTART", fontPath, 28);
    restartLabel->SetPosition((WIDTH - restartLabel->GetWidth()) / 2 + 20, 540);
    UI.push_back(restartLabel);

    Text* quitLabel = new Text();
    quitLabel->Initialize("QUIT", fontPath, 28);
    quitLabel->SetPosition((WIDTH - quitLabel->GetWidth()) / 2 + 20, 630);
    UI.push_back(quitLabel);

    RefreshStatsText();
    return true;
}

void GameOverScene::RefreshStatsText() 
{
    timeText->SetText("Time Lived: " + to_string(minutes) + "m " + to_string(seconds) + "s");
    stageText->SetText("Stages Completed: " + to_string(stagesCompleted));
    itemsText->SetText("Items Collected: " + to_string(itemsCollected));
    coinsText->SetText("Coins Left After Death: " + to_string(coinsLeft));
}

void GameOverScene::SetStats(int minutes, int seconds, int stagesCompleted, int itemsCollected, int coinsLeft) 
{
    this->minutes = minutes;
    this->seconds = seconds;
    this->stagesCompleted = stagesCompleted;
    this->itemsCollected = itemsCollected;
    this->coinsLeft = coinsLeft;
    RefreshStatsText();
}

void GameOverScene::Process(float deltaTime) 
{
    Scene::Process(deltaTime);
}

void GameOverScene::Draw(Renderer* renderer) 
{
    Scene::Draw(renderer);
}

bool GameOverScene::RestartPressedFalse() //Resets the restart button so that it does not immediately restart the game
{
    return restartPressed = false;
}
bool GameOverScene::RestartPressed()
{
    return restartPressed;
}
bool GameOverScene::QuitPressed()
{
    return quitPressed;
}