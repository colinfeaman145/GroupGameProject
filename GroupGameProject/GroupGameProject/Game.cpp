#include <SDL.h>
#include <SDL_image.h>
#include "game.hpp"
#include "SplashScreenScene.hpp"
#include "TestingAreaScene.hpp"
#include "ItemRegistry.hpp"
#include "GameContext.hpp"
#include "DifficultyCalculator.hpp"
#include "Camera.hpp"
#include "GameOverScene.hpp"
#include "Player.hpp"
#include "MainMenuScene.hpp"
#include "InstructionsScene.hpp"
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
    FMOD_VECTOR pos = { 0,0,0 };
    context.am->Initialize(pos, 5);
	context.ir = new ItemRegistry();
    context.timer = new GameTimer();
    context.dc = new DifficultyCalculator();
    context.er = new EntityRegistry();
    context.im->Initialize("../../data/inputs.json");
    context.changeScene = [this](int i) { this->ChangeScene(i); };

    SetupItemRegistry("../../data/items.json");
    SetupEntityRegistry("../../data/entities.json");

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

    context.am->AddGroup("Music");
    context.am->AddGroup("UI");
    context.am->AddGroup("SFX");

    // main game scenes
    Scene* splash;
    splash = new SplashScreens();
    splash->Initialize();
    scenes[0] = splash;

    // main menu scene
    Scene* mainMenu;
    mainMenu = new MainMenuScene();
    mainMenu->Initialize();
    scenes[1] = mainMenu;

    // instructions scene
    Scene* Instructions;
    Instructions = new InstructionsScene();
    Instructions->Initialize();
    scenes[2] = Instructions;

    // testing area
    Scene* game;
    game = new TestingAreaScene();
    game->Initialize();
    scenes[3] = game;

    // game over scene
    Scene* gameOver;
    gameOver = new GameOverScene();
    gameOver->Initialize();
    scenes[4] = gameOver;

    
    context.am->LoadMusicTrack("../../assets/audio/menu_music.wav", "menu_music");
    context.am->PlayMusic("menu_music");
    context.am->SetGroupVolume("UI", 0.8f);
    context.am->SetGroupVolume("SFX", 0.8f);

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

    FMOD_VECTOR pos = { 0,0,0 };
    context.am->Process({ 0,0 }, deltaTime);

   scenes[currentScene]->Process(deltaTime);

   if (currentScene == 0 && static_cast<SplashScreens*>(scenes[currentScene])->IsDone() == true)//checks if the splash screens are done then changes the scene
   {
       ChangeScene(1);
   }

   if (currentScene == 1) //checks if current scene is in main menu scene first
   {
       if (static_cast<MainMenuScene*>(scenes[currentScene])->StartPressed() == true)
       {
           ChangeScene(3);
       }
       if (static_cast<MainMenuScene*>(scenes[currentScene])->InstructionsPressed() == true)
       {
           static_cast<MainMenuScene*>(scenes[1])->InstructionsPressedFalse();//change it back to false so that the player can repeatedly go to the instructions
           ChangeScene(2);
       }
       if (static_cast<MainMenuScene*>(scenes[currentScene])->QuitPressed() == true)
       {
           Quit();
       }
   }
   
   if (currentScene == 2 && static_cast<InstructionsScene*>(scenes[currentScene])->BackPressed() == true)
   {
       static_cast<InstructionsScene*>(scenes[2])->BackPressedFalse();
       ChangeScene(1);
   }

   if (currentScene == 3) //checks if current scene is in testing area scene first
   {
       auto testingArea = static_cast<TestingAreaScene*>(scenes[currentScene]);
       Player* player = testingArea->GetPlayer();
       
       if (player && player->IsAlive() == false) //checks if player is dead
       {
           int mins = context.timer->ElapsedMinutes();
           int secs = (int)context.timer->ElapsedSeconds() % 60;
           int stages = context.dc->GetStagesCompleted();
           int items = 0;
           auto totalItems = player->GetItems();
           for (auto item = totalItems.begin(); item != totalItems.end(); ++item) 
           {
               if (item->first == 1 || item->first == 3 || item->first == 4 || item->first == 5 || item->first == 7 || item->first == 8) //excluding the initial gun, the coins, the cashoutmodule, invinciblemodule, levelscaler and the key from the item count
               {
                   continue;
               }
               if (item->second > 0)
               {
                   items++;
               }
           }
           int coins = player->GetItemCount(3);

           static_cast<GameOverScene*>(scenes[4])->SetStats(mins, secs, stages, items, coins);
           ChangeScene(4);
       }
   }

   if (currentScene == 4) //checks if current scene is in game over scene first
   {
       if (static_cast<GameOverScene*>(scenes[currentScene])->RestartPressed() == true)
       {
           static_cast<GameOverScene*>(scenes[4])->RestartPressedFalse();
           auto testingArea = static_cast<TestingAreaScene*>(scenes[3]);
           testingArea->Initialize();
           ChangeScene(3);
       }
       if (static_cast<GameOverScene*>(scenes[currentScene])->QuitPressed() == true)
       {
           Quit();
       }
   }
   

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