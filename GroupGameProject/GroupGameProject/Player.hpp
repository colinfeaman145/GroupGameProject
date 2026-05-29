#pragma once
#include "Attackable.hpp"

class AnimatedSprite;
class Player : public Attackable {

public:
	void Initialize(Vector2 pos);
	void Process(float deltaTime) override;
	void Draw(Renderer* renderer) override;
	void HandleCollision(Collidable* other, Vector2 penetration) override;

private:
	void HandleAnimation();
	void HandleMouseClick(float deltaTime);
	void HandleMovement();


private:
	int coinCount;
	float attackCooldown;
};

