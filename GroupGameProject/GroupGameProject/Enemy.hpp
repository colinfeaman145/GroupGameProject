#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Attackable.hpp"
#include "AI.hpp"
#include "GridCoord.hpp"

enum class EnemyType {
	FLYING,//GREY
	GROUND,//GREEN
};


class AnimatedSprite;
class Enemy : public AI {
public:
	Enemy();
	~Enemy();

	// lifecycle functions
	virtual void Initialize(Vector2 pos);
	virtual void Draw(Renderer* renderer) override;
	virtual void Process(float deltaTime) override;
	virtual void HandleCollision(Collidable* other, Vector2 penetration) override;
	virtual void OnStuck() override;


	// getter
	float GetAttackCooldown();

	//setter
	void SetAttackCooldown(float atckCool);




private:
	EnemyType type;
	Entity* currentTarget;

	float attackCooldown;
	float currentAttackCooldown;
};

#endif