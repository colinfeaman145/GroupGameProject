#pragma once
#include "Entity.hpp"
#include "ItemEffect.hpp"
#include  <string>

class Attackable;
class AnimatedSprite;
class Explosion : public Entity {
public:
	Explosion();
	bool Initialize(EventContext ctx, float radius);
	void Draw(Renderer* renderer) override;
	void Process(float deltaTime) override;
	void HandleCollision(Collidable* other, Vector2 penetration) override;

private:
	float damage;
	float radius;
	AnimatedSprite* explosionAnimation;
	Attackable* source;
	vector<Collidable*> collisions;//to prevent multiple collisions with the same target
};
