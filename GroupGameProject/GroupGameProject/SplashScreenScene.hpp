#ifndef SPLASH_SCREENS_HPP
#define SPLASH_SCREENS_HPP

#include "Scene.hpp"

using namespace std;
class SplashScreens : public Scene {

public:
    SplashScreens() {}

    bool Initialize() override;
    void Process(float deltaTime) override;
    void Draw(Renderer* renderer) override;
    void ReadInputs(float deltaTime);

    bool IsDone();

protected:
    vector<Element*> elements;
    vector<Sprite*> UI;

    Sprite* autLogo;
    Sprite* fmodLogo;
    Sprite* TeamLogo;
    float timer;

    bool isdone;

};

#endif