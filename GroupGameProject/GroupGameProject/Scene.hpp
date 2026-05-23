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

public:
    Scene() {}
    virtual ~Scene() {}

	virtual bool Initialize() { return true; } //makes and saves sprites and entities
    virtual void Process(float deltaTime) {
		// fill in elements to add from last frame
		if (!elementsToAdd.empty()) {
			for (Element* e : elementsToAdd) {
				elements.push_back(e);
			}
			elementsToAdd.clear();
		}
		// fill in elements to delete from last frame
		if (!elementsToDelete.empty()) {
			for (Element* e : elementsToDelete) {
				auto it = find(elements.begin(), elements.end(), e);
				if (it != elements.end()) {
					elements.erase(it);
				}
			}
			elementsToDelete.clear();
		}
		for (Element* e : elements) {
			e->Process(deltaTime);
			if (e->isToBeDeleted) {
				elementsToDelete.push_back(e);
			}
		}
    }
	virtual void Draw(Renderer* renderer) {
		for (Element* e : elements) {
			e->Draw(renderer);
		}
	}
	virtual void ReadInputs(float deltaTime) {};
	virtual void AddElement(Element* e) { elementsToAdd.push_back(e); }

protected:
    vector<Sprite*> UI;
    vector<Element*> elements;

    // list for items to add/remove next frame
    // add/remove new items to this instead of elements during runtime
    vector<Element*> elementsToAdd;
    vector<Element*> elementsToDelete;

};

#endif