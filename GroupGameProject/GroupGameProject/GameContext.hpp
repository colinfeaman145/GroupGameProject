#ifndef GAMECONTEXT_HPP
#define GAMECONTEXT_HPP

#include <random>
#include <functional>
#include "Renderer.hpp"
#include "TextureManager.hpp"
#include "FontManager.hpp"
#include "AudioManager.hpp"
#include "InputManager.hpp"
#include "ItemRegistry.hpp"
#include "EntityRegistry.hpp"
#include "DifficultyCalculator.hpp"
#include "Grid.hpp"
#include "Scene.hpp"
#include "GameTimer.hpp"
class Grid;


#define DEBUGMODE true
#define GOD_MODE true


struct GameContext {
    Renderer* renderer;
    TextureManager* txm;
    FontManager* fm;
    AudioManager* am;
    InputManager* im;
	ItemRegistry* ir;
    EntityRegistry* er;
    GameTimer* timer;
    DifficultyCalculator* dc;
    Grid* grid;

    function<void(int)> changeScene;
};

inline GameContext context;
inline mt19937 gen(random_device{}());

#endif