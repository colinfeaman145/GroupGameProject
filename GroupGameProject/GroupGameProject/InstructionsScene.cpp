#include "InstructionsScene.hpp"
#include "Sprite.hpp"
#include "Camera.hpp"

InstructionsScene::InstructionsScene()
    : titleText(nullptr)
    , backButton(nullptr)
    , backPressed(false)
{
}

InstructionsScene::~InstructionsScene()
{
}

bool InstructionsScene::Initialize()
{
    const string fontPath = "../../assets/fonts/pixelpurl/PixelPurl.ttf";

    titleText = new Text();
    titleText->Initialize("Instructions", fontPath, 150);
    titleText->SetPosition((WIDTH - titleText->GetWidth()) / 2, 30);
    UI.push_back(titleText);

    Text* Text1 = new Text();
    Text1->Initialize("INPUT CONTROLS:", fontPath, 40);
    Text1->SetPosition((WIDTH - Text1->GetWidth()) / 2, 170);
    UI.push_back(Text1);

    Text* Text2 = new Text();
    Text2->Initialize("W/A/S/D - Move.", fontPath, 28);
    Text2->SetPosition((WIDTH - Text2->GetWidth()) / 2, 220);
    UI.push_back(Text2);

    Text* Text3 = new Text();
    Text3->Initialize("Mouse - Aim.", fontPath, 28);
    Text3->SetPosition((WIDTH - Text3->GetWidth()) / 2, 250);
    UI.push_back(Text3);

    Text* Text4 = new Text();
    Text4->Initialize("Left Click - Shoot.", fontPath, 28);
    Text4->SetPosition((WIDTH - Text4->GetWidth()) / 2, 280);
    UI.push_back(Text4);

    Text* Text5 = new Text();
    Text5->Initialize("HOW TO PLAY:", fontPath, 40);
    Text5->SetPosition((WIDTH - titleText->GetWidth()) / 2, 330);
    UI.push_back(Text5);

    Text* Text6 = new Text();
    Text6->Initialize("1. Move around the rooms and dodge enemies.", fontPath, 28);
    Text6->SetPosition((WIDTH - titleText->GetWidth()) / 2, 380);
    UI.push_back(Text6);

    Text* Text7 = new Text();
    Text7->Initialize("2. Aim with you mouse and shoot enemies.", fontPath, 28);
    Text7->SetPosition((WIDTH - titleText->GetWidth()) / 2, 410);
    UI.push_back(Text7);

    Text* Text8 = new Text();
    Text8->Initialize("3. Pick up coins and resources to unlock perks and abilities.", fontPath, 28);
    Text8->SetPosition((WIDTH - titleText->GetWidth()) / 2, 440);
    UI.push_back(Text8);

    Text* Text9 = new Text();
    Text9->Initialize("4. Go through a door to the next room.", fontPath, 28);
    Text9->SetPosition((WIDTH - titleText->GetWidth()) / 2, 470);
    UI.push_back(Text9);

    Text* Text10 = new Text();
    Text10->Initialize("5. Keep clearing rooms until you reach the boss.", fontPath, 28);
    Text10->SetPosition((WIDTH - titleText->GetWidth()) / 2, 500);
    UI.push_back(Text10);

    Text* Text11 = new Text();
    Text11->Initialize("6. Beat the boss to win.", fontPath, 28);
    Text11->SetPosition((WIDTH - titleText->GetWidth()) / 2, 530);
    UI.push_back(Text11);

    backButton = new Button(440, 590, 220, 70, { 40, 40, 40, 255 }, { 80, 80, 80, 255 }, { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, 4, [this]() { backPressed = true; }, 1.05f);
    backButton->SetPosition((WIDTH - backButton->GetWidth()) / 2, 580);
    UI.push_back(backButton);

    Text* backLabel = new Text();
    backLabel->Initialize("BACK", fontPath, 28);
    backLabel->SetPosition((WIDTH - backLabel->GetWidth()) / 2, 600);
    UI.push_back(backLabel);

    return true;
}

void InstructionsScene::Process(float deltaTime)
{
    Scene::Process(deltaTime);
}

void InstructionsScene::Draw(Renderer* renderer)
{
    Scene::Draw(renderer);
}

bool InstructionsScene::BackPressed()
{
    return backPressed;
}
bool InstructionsScene::BackPressedFalse()
{
    return backPressed = false;
}
