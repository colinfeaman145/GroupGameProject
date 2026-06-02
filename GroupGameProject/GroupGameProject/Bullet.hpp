#pragma once
#include "Entity.hpp"
#include "ItemEffect.hpp"
#include  <string>

class Attackable;
class AnimatedSprite;
class Bullet : public Entity {
public:
	Bullet(std::string spritePath);
	bool Initialize(EventContext ctx, int pierceCount, float ttl, float bulletSpeed, float rotation, AnimatedSprite* spr = nullptr);
	void Draw(Renderer* renderer) override;
	void Process(float deltaTime) override;
	void HandleCollision(Collidable* other, Vector2 penetration) override;

private:
	float damage;
	float ttl;
	float bulletSpeed;
	AnimatedSprite* bulletSprite;

	Attackable* source;
	int pierceCount; //for piercing bullets, can be implemented later

	vector<Collidable*> collisions;//to prevent multiple collisions with the same target
};
