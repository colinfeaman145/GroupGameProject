#include "MainMenuScene.hpp"
#include "Sprite.hpp"
#include "Camera.hpp"

MainMenuScene::MainMenuScene()
    : titleText(nullptr)
    , startButton(nullptr)
    , instructionsButton(nullptr)
    , quitButton(nullptr)
    , startPressed(false)
    , instructionsPressed(false)
    , quitPressed(false)
{
}

MainMenuScene::~MainMenuScene()
{
}

bool MainMenuScene::Initialize()
{
    const string fontPath = "../../assets/fonts/pixelpurl/PixelPurl.ttf";

    titleText = new Text();
    titleText->Initialize("Dungeon Descent", fontPath, 200);
    titleText->SetPosition((WIDTH - titleText->GetWidth()) / 2, 80);
    UI.push_back(titleText);

    startButton = new Button(440, 500, 220, 70, { 40, 40, 40, 255 }, { 80, 80, 80, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, 4, [this]() { startPressed = true; }, 1.05f);
    startButton->SetPosition((WIDTH - startButton->GetWidth()) / 2, 400);
    UI.push_back(startButton);

    instructionsButton = new Button(440, 500, 220, 70, { 40, 40, 40, 255 }, { 80, 80, 80, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, 4, [this]() { instructionsPressed = true; }, 1.05f);
    instructionsButton->SetPosition((WIDTH - instructionsButton->GetWidth()) / 2, 490);
    UI.push_back(instructionsButton);

    quitButton = new Button(440, 590, 220, 70, { 40, 40, 40, 255 }, { 80, 80, 80, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, 4, [this]() { quitPressed = true; }, 1.05f);
    quitButton->SetPosition((WIDTH - quitButton->GetWidth()) / 2, 580);
    UI.push_back(quitButton);

    Text* startLabel = new Text();
    startLabel->Initialize("START", fontPath, 28);
    startLabel->SetPosition((WIDTH - startLabel->GetWidth()) / 2, 420);
    UI.push_back(startLabel);

    Text* instructionLabel = new Text();
    instructionLabel->Initialize("INSTRUCTIONS", fontPath, 28);
    instructionLabel->SetPosition((WIDTH - instructionLabel->GetWidth()) / 2, 510);
    UI.push_back(instructionLabel);

    Text* quitLabel = new Text();
    quitLabel->Initialize("QUIT", fontPath, 28);
    quitLabel->SetPosition((WIDTH - quitLabel->GetWidth()) / 2, 600);
    UI.push_back(quitLabel);

    return true;
}

void MainMenuScene::Process(float deltaTime)
{
    Scene::Process(deltaTime);
}

void MainMenuScene::Draw(Renderer* renderer)
{
    Scene::Draw(renderer);
}

bool MainMenuScene::StartPressed()
{
    return startPressed;
}

bool MainMenuScene::InstructionsPressed()
{
    return instructionsPressed;
}
bool MainMenuScene::InstructionsPressedFalse()
{
    return instructionsPressed = false;
}
bool MainMenuScene::QuitPressed()
{
    return quitPressed;
}