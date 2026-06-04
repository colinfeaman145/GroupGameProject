#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "AI.hpp"
#include "GridCoord.hpp"

enum class EnemyType {
	FLYING,//GREY
	GROUND,//GREEN
};

enum EnemyState {
	IDLE,
	APPROACHING,
	ATTACKING,
	RECOVERING
};

class AnimatedSprite;
class Enemy : public AI {
public:
	Enemy();
	~Enemy();

	// lifecycle functions
	virtual bool Initialize(Vector2 pos, Sprite* = nullptr) override;
	virtual void Draw(Renderer* renderer) override;
	virtual void Process(float deltaTime) override;
	virtual void HandleCollision(Collidable* other, Vector2 penetration) override;
	virtual void OnStuck() override;
	bool IsTargetInAttackRange();



	// getter
	float GetAttackCooldown();

	//setter
	void SetAttackCooldown(float atckCool);
	void ChangeState(EnemyState newState);




protected:
	EnemyType type;
	EnemyState currentState;
	Entity* currentTarget;

	float attackCooldown;
	float currentAttackCooldown;
};

#endif