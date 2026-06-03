
#ifndef GAMEOVERSCENE_HPP
#define GAMEOVERSCENE_HPP

#include "Scene.hpp"
#include "Button.hpp"
#include "Text.hpp"

class GameOverScene : public Scene {
public:
    GameOverScene();
    ~GameOverScene();

    bool Initialize();
    void Process(float deltaTime);
    void Draw(Renderer* renderer);
    bool RestartPressedFalse();
    bool RestartPressed();
    bool QuitPressed();
    void SetStats(int minutes, int seconds, int stagesCompleted, int itemsCollected, int coinsLeft);

private:
    void RefreshStatsText();

    Text* titleText;
    Text* timeText;
    Text* stageText;
    Text* itemsText;
    Text* coinsText;

    Button* restartButton;
    Button* quitButton;

    Sprite* autLogo;

    int minutes;
    int seconds;
    int stagesCompleted;
    int itemsCollected;
    int coinsLeft;

    bool restartPressed;
    bool quitPressed;
};

#endif // GAMEOVERSCRENE_HPP
