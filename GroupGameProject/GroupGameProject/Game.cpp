#include <SDL.h>
#include <SDL_image.h>
#include "game.hpp"
#include "SplashScreenScene.hpp"
#include "TestingAreaScene.hpp"

/*
CALL PIPELINE
Main makes game
Game makes renderer and scenes
Each frame, game draws current scene
Scene draws out sprites
Sprites call drawTexture to renderer
*/

Game::Game() {
    //define context
    context.renderer = new Renderer();
    context.renderer->Initialize("GAME", WIDTH, HEIGHT, true);
    context.txm = new TextureManager();
    context.fm = new FontManager();
    context.im = new InputManager();
    context.am = new AudioManager();
    context.im->Initialize("../../data/inputs.json");
    context.changeScene = [this](int i) { this->ChangeScene(i); };

    currentScene = 0;
    running = true;

}

Game::~Game() {
    for (size_t i = 0; i < scenes.size(); ++i) {
        delete scenes[i];
        scenes[i] = nullptr;
    }
    scenes.clear();

    IMG_Quit();
}

//add scenes to game
bool Game::Initialize() {

    scenes.resize(10, nullptr);//make space for atleast 10 scenes

    // main game scenes
    Scene* splash;
    splash = new SplashScreens();
    splash->Initialize();
    scenes[0] = splash;

    Scene* mainMenu;
    mainMenu = new TestingAreaScene();
    mainMenu->Initialize();
    scenes[1] = mainMenu;


    // testing area
    Scene* game;
    game = new TestingAreaScene();
    game->Initialize();
    scenes[9] = game;

    return true;
}

void Game::Run() {

    Uint32 lastTick = SDL_GetTicks();

    while (running) {
        Uint32 current = SDL_GetTicks();
        float deltaTime = (current - lastTick) / 1000.0f;
        lastTick = current;

        Process(deltaTime);
        Draw();
    }

    SDL_Quit();
}

void Game::Process(float deltaTime) {

    context.im->Process();//process inputs
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }

        context.im->HandleEvent(event);
    }

    scenes[currentScene]->Process(deltaTime);
}

void Game::Draw()
{
    context.renderer->BeginFrame();
    scenes[currentScene]->Draw(context.renderer);
    context.renderer->EndFrame();
}

void Game::Cleanup()
{
}

void Game::ChangeScene(int s) {
    if (s == -1) Quit();

    if (s < 0 || s >= (int)scenes.size()) return;
    currentScene = s;
}

void Game::Quit() {
    running = false;
}