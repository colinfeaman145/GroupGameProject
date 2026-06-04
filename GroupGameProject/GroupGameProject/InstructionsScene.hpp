#ifndef INSTRUCTIONSSCENE_HPP
#define INSTRUCTIONSSCENE_HPP

#include "Scene.hpp"
#include "Button.hpp"
#include "Text.hpp"

class InstructionsScene : public Scene {
public:
    InstructionsScene();
    ~InstructionsScene();

    bool Initialize();
    void Process(float deltaTime);
    void Draw(Renderer* renderer);
    bool BackPressed();
    bool BackPressedFalse();

private:
    Text* titleText;
    Button* backButton;
    bool backPressed;
};

#endif // INSTRUCTIONSSCENE_HPP
