#pragma once
#include "Enemy.hpp"
#include "Vector2.hpp"



class FlyingDevil : public Enemy {
public:
	virtual void Initialize(Vector2 pos) override;
	void Process(float deltaTime) override;
	void Approach(float deltaTime);
	void Recover(float deltaTime);
	void HandleCollision(Collidable* other, Vector2 penetration) override;
	void HandleAttackAnimation();
	void HandleRecoverAnimation();
	void RecalculateDirection();

private:
	float maxSpeed;
	float acceleration;
	float deceleration;
	Vector2 currentTargetPosition;
};

