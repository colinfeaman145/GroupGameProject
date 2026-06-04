#pragma once
#include "Enemy.hpp"

class EnemyId_3 : public Enemy {
public:
	// lifecycle functions
	bool Initialize(Vector2 pos, Sprite* spr = nullptr) override;
	void Draw(Renderer* renderer) override;
	void Process(float deltaTime) override;
	void HandleCollision(Collidable* other, Vector2 penetration) override;
	void RecalculateTargetLocation();

private:
	void HandleAttack();

private:
	Vector2 currentTargetPos;

};

