#ifndef SCENE_HPP
#define SCENE_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Entity.hpp"
#include "Text.hpp"
#include "Sprite.hpp"
#include "AnimatedSprite.hpp"
#include "ParticleEmitter.hpp"
#include "GameContext.hpp"

using namespace std;
class Scene {
protected:
    vector<Element*> elements;
    vector<Sprite*> UI;
public:
    Scene() {}
    virtual ~Scene() {}

    virtual bool Initialize() = 0; //makes and saves sprites and entities
    virtual void Process(float deltaTime) = 0;
    virtual void Draw(Renderer* renderer) = 0;

    virtual void ReadInputs(float deltaTime) = 0;
};

#endif