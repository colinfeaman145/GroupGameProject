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

class Grid;
class Scene;

#define DEBUGMODE false
#define GOD_MODE true

struct GameContext {
    Renderer* renderer;
    TextureManager* txm;
    FontManager* fm;
    AudioManager* am;
    InputManager* im;
	ItemRegistry* ir;
	Scene* currentScene;
    Grid* grid;
    function<void(int)> changeScene;
};

inline GameContext context;
inline mt19937 gen(random_device{}());

#endif