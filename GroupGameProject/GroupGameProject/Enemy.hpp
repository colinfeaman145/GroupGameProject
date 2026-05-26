#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Attackable.hpp"
#include "GridCoord.hpp"

enum class EnemyType {
	WALL_FOCUS,//GREY
	NORMAL,//GREEN
	PLAYER_FOCUS,//BLUE
	FAST,//PEACH
	ATLAS_FOCUS,//ORANGE
	EXPLOSIVE,//RED
	RANDOM//PINK = random
};


class AnimatedSprite;
class Enemy : public Attackable {
public:
	Enemy();
	~Enemy();

	// lifecycle functions
	void Initialize(Vector2 pos, float retarget, int targetRad, float atlasTarget, float playerTarget);
	void Draw(Renderer* renderer) override;
	void Process(float deltaTime) override;
	void HandleCollision(Collidable* other, Vector2 penetration) override;


	// getter
	EnemyType GetType();
	int GetDamage();
	float GetAttackCooldown();
	int GetDropAmount() const;

	//setter
	void SetType(EnemyType t);
	void SetDamage(int d);
	void SetAttackCooldown(float atckCool);
	void SetKilledByPlayer();




private:
	GridCoord lastCell;
	bool killedByPlayer;

	EnemyType type;
	int damage;
	float attackCooldown;
	float currentAttackCooldown;
};

#endif