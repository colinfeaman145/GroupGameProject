#include "SplashScreenScene.hpp"

bool SplashScreens::Initialize() {
    timer = 0.0f;

    SDL_Texture* autTex = context.txm->LoadTexture(context.renderer, "../../assets/sprites/SplashScreens/AUT_logo.png");
    SDL_Texture* fmodTex = context.txm->LoadTexture(context.renderer, "../../assets/sprites/SplashScreens/FMOD Logo White - Black Background.png");

    autLogo = new Sprite();
    autLogo->Initialize(autTex, 375, 265, 0, 0, WIDTH * 1.5, HEIGHT * 2);
    autLogo->SetPosition(Vector2((autLogo->GetWidth() - WIDTH) / 2, (autLogo->GetHeight() / HEIGHT) / 2));

    fmodLogo = new Sprite();
    fmodLogo->Initialize(fmodTex, 728, 192, 0, 0, WIDTH * 1.5, HEIGHT);
    fmodLogo->SetPosition(Vector2((fmodLogo->GetWidth() - WIDTH) / 2, HEIGHT * 0.5));

    return true;
}

void SplashScreens::Process(float deltaTime) {
    ReadInputs(deltaTime);
    timer += deltaTime;
    if (timer >= 4 * 2) context.changeScene(0);

    if (timer > 8) //once both logos have finished, set as done so it can move on to next scene(without it, the fmod would keep on flashing)
    {
        isdone = true;
    }
}

void SplashScreens::Draw(Renderer* renderer) {
    float t = timer < 4 ? timer : timer - 4;
    Sprite* logo = timer < 4 ? autLogo : fmodLogo;

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