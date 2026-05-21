#pragma once
#include "AnimatedSprite.hpp"
#include "Attackable.hpp"

class Player : public Attackable {

public:
	void Initialize(Vector2 pos, AnimatedSprite* spr);
	void Process(float deltaTime) override;
	void Draw(Renderer* renderer) override;
	void HandleCollision(Collidable* other, Vector2 penetration) override;
	void SetSprites(AnimatedSprite* move, AnimatedSprite* idle);

private:
	void HandleAnimation();
	void HandleMovement();


private:
	AnimatedSprite* moving;
	AnimatedSprite* idle;

};

