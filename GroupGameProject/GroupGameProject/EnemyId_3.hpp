#pragma once
#include "Enemy.hpp"

class EnemyId_3 : public Enemy {
public:
	// lifecycle functions
	bool Initialize(Vector2 pos, Sprite* spr = nullptr) override;
	void Draw(Renderer* renderer) override;
	void Process(float deltaTime) override;
	void UpdateState();
	void HandleCollision(Collidable* other, Vector2 penetration) override;
	void EnterState(EnemyState newState);

	void DoAttack();

private:
	Vector2 currentTargetPos;

};

