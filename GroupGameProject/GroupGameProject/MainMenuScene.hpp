#ifndef MAINMENUSCENE_HPP
#define MAINMENUSCENE_HPP

#include "Scene.hpp"
#include "Button.hpp"
#include "Text.hpp"

class MainMenuScene : public Scene {
public:
    MainMenuScene();
    ~MainMenuScene();

    bool Initialize();
    void Process(float deltaTime);
    void Draw(Renderer* renderer);
    bool StartPressed();
    bool InstructionsPressed();
    bool InstructionsPressedFalse();
    bool QuitPressed();

private:
    Text* titleText;

    Button* startButton;
    Button* instructionsButton;
    Button* quitButton;

    bool startPressed;
    bool instructionsPressed;
    bool quitPressed;
};

#endif // MAINMENUSCRENE_HPP
