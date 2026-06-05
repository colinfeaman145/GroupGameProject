#pragma once
#include "Enemy.hpp"
#include "Vector2.hpp"



class EnemyId_2 : public Enemy {
public:
	bool Initialize(Vector2 pos, Sprite* spr = nullptr) override;
	void Process(float deltaTime) override;
	void UpdateState();
	void EnterState(EnemyState newState);
	void DoAttack();
	void HandleCollision(Collidable* other, Vector2 penetration) override;
};

