#include "Prop.hpp"


bool Prop::Initialize(std::string name, Vector2 pos, Sprite* spr) {
	this->name = name;
	Entity::Initialize(pos, spr);
	return true;
}

void Prop::Draw(Renderer* renderer) {
	Entity::Draw(renderer);
}

void Prop::Process(float deltaTime) {
	Entity::Process(deltaTime);
}

void Prop::HandleCollision(Collidable* other, Vector2 penetration) {
}
