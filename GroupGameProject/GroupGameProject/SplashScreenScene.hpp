#ifndef SPLASH_SCREENS_HPP
#define SPLASH_SCREENS_HPP

#include "Scene.hpp"

using namespace std;
class SplashScreens : public Scene {
protected:
    vector<Element*> elements;
    vector<Sprite*> UI;

    Sprite* autLogo;
    Sprite* fmodLogo;
    float timer;

public:
    SplashScreens() {}
    ~SplashScreens() {}

    bool Initialize() override;
    void Process(float deltaTime) override;
    void Draw(Renderer* renderer) override;

    void ReadInputs(float deltaTime) override;

};

#endif