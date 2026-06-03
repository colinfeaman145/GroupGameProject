#pragma once
#include "Entity.hpp"
#include <string>

class Prop : public Entity {

public:	
	// Inherited via Entity
	virtual bool Initialize(std::string name, Vector2 pos, Sprite* spr = nullptr);
	virtual void Draw(Renderer* renderer) override;
	virtual void Process(float deltaTime) override;
	void HandleCollision(Collidable* other, Vector2 penetration) override;

public:
	std::string name;

};

