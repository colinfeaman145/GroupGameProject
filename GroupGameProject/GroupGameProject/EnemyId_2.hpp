#pragma once
#include "Enemy.hpp"
#include "Vector2.hpp"



class EnemyId_2 : public Enemy {
public:
	bool Initialize(Vector2 pos, Sprite* spr = nullptr) override;
	void Process(float deltaTime) override;
	void HandleCollision(Collidable* other, Vector2 penetration) override;
	void HandleAttackAnimation();
	void HandleRecoverAnimation();

private:
	float maxSpeed;
	float acceleration;
	float deceleration;
	float moveSpeed;
	Vector2 currentTargetPosition;
};

