#pragma once
#include "Attackable.hpp"

class AnimatedSprite;
class Player : public Attackable {

public:
	Player();
	void Initialize(Vector2 pos);
	void Process(float deltaTime) override;
	void Draw(Renderer* renderer) override;
	void HandleCollision(Collidable* other, Vector2 penetration) override;
	int GetCoinCount();

private:
	void HandleAnimation();
	void HandleMouseClick();
	void HandleMovement();


private:
	int coinCount;
};

