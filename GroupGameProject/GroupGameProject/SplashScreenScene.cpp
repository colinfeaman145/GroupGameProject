#include "SplashScreenScene.hpp"
#include "GameContext.hpp"
#include "Sprite.hpp"
#include "Camera.hpp"

bool SplashScreens::Initialize() {
    timer = 0.0f;

    SDL_Texture* autTex = context.txm->LoadTexture(context.renderer, "../../assets/sprites/SplashScreens/AUT_logo.png");
    SDL_Texture* fmodTex = context.txm->LoadTexture(context.renderer, "../../assets/sprites/SplashScreens/FMOD Logo White - Black Background.png");
    SDL_Texture* TeamTex = context.txm->LoadTexture(context.renderer, "../../assets/sprites/SplashScreens/Team_Splash.png");

    autLogo = new Sprite();
    autLogo->Initialize(autTex, 375, 265, 0, 0, WIDTH * 2, HEIGHT * 2);
    autLogo->SetPosition((autLogo->GetWidth()) / 2, (autLogo->GetHeight()) / 2);
    
    fmodLogo = new Sprite();
    fmodLogo->Initialize(fmodTex, 728, 192, 0, 0, WIDTH * 2, HEIGHT);
    fmodLogo->SetPosition((fmodLogo->GetWidth()) / 2, (fmodLogo->GetHeight()) / 2 + 800);

    TeamLogo = new Sprite();
    TeamLogo->Initialize(TeamTex, 550, 294, 0, 0, WIDTH * 2, HEIGHT * 2);
    TeamLogo->SetPosition((TeamLogo->GetWidth()) / 2 , (TeamLogo->GetHeight()) / 2);

    return true;
}

void SplashScreens::Process(float deltaTime) {
    ReadInputs(deltaTime);
    timer += deltaTime;
    if (timer >= 4 * 3) context.changeScene(0);

    if (timer > 12) //once all splashes have finished, set as done so it can move on to next scene(without it, the fmod would keep on flashing)
    {
        isdone = true;
    }
}

void SplashScreens::Draw(Renderer* renderer) {
    float t = timer < 4 ? timer : timer < 8 ? timer - 4 : timer - 8;
    Sprite* logo = timer < 4 ? autLogo : timer < 8 ? fmodLogo : TeamLogo;

    //increase then decrease
    float alpha = t < 2 ? (t / 2) * 255 : (1 - (t - 2) / 2) * 255;

    logo->SetAlpha((int)alpha);
    logo->Draw(renderer);
}

void SplashScreens::ReadInputs(float deltaTime) {
	if (context.im->IsKeyPressed("testing_room"))
		context.changeScene(9);
}
bool SplashScreens::IsDone()
{
    return isdone;
}