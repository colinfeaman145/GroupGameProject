#include "Scene.hpp"
#include "Entity.hpp"
#include "Text.hpp"
#include "Sprite.hpp"
#include "AnimatedSprite.hpp"
#include "ParticleEmitter.hpp"
#include "GameContext.hpp"
#include "Player.hpp"
#include "Container.hpp"

Scene::Scene() {}
Scene::~Scene() {
	for (Element* e : elements) {
		delete e;
	}
	elements.clear();
	for (Sprite* s : UI) {
		delete s;
	}
	UI.clear();
}

bool Scene::Initialize() { return true; }

void Scene::Process(float deltaTime) {
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

	for (Sprite* s : UI) {
		s->Process(deltaTime);
	}

}
void Scene::Draw(Renderer* renderer) {
	for (Element* e : elements) {
		e->Draw(renderer);
	}
	for (Sprite* s : UI) {
		s->Draw(renderer);
	}
}

void Scene::ReadInputs(float deltaTime) {};

void Scene::AddElement(Element* e) { 
	elementsToAdd.push_back(e);
}

