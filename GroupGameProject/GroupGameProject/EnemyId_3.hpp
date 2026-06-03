#pragma once
#include "Enemy.hpp"

class EnemyId_3 : public Enemy {
public:
	// lifecycle functions
	virtual void Initialize(Vector2 pos);
	virtual void Draw(Renderer* renderer) override;
	virtual void Process(float deltaTime) override;
	virtual void HandleCollision(Collidable* other, Vector2 penetration) override;
	void RecalculateTargetLocation();

private:
	void HandleAttack();

private:
	Vector2 currentTargetPos;

};

