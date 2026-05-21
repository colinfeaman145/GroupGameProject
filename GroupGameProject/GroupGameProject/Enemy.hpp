#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"
#include "GridCoord.hpp"
#include "AnimatedSprite.hpp"
#include "Attackable.hpp"

//#include "Explosion.hpp"

enum class EnemyType {
	WALL_FOCUS,//GREY
	NORMAL,//GREEN
	PLAYER_FOCUS,//BLUE
	FAST,//PEACH
	ATLAS_FOCUS,//ORANGE
	EXPLOSIVE,//RED
	RANDOM//PINK = random
};

class Enemy : public Attackable {
public:
	Enemy();
	~Enemy();
	void Initialize(Vector2 pos, AnimatedSprite* spr, float retarget, int targetRad, float atlasTarget, float playerTarget);
	void Draw(Renderer* renderer) override;
	void Process(float deltaTime) override;
	void SetSprites(AnimatedSprite* move, AnimatedSprite* attack, AnimatedSprite* die);
	void SetSpritesDrawSize(int size);
	void SetSpriteDirection(bool b);
	void SetType(EnemyType t);
	EnemyType GetType();
	void SetDamage(int d);
	int GetDamage();
	void SetDead();
	void Damage(float amount);
	bool IsDying();
	void SetAttackCooldown(float atckCool);
	
	float GetAttackCooldown();
	void SetKilledByPlayer();
	//ResourceType GetDropType() const;
	int GetDropAmount() const;

	void Attack(Attackable* a);

	void HandleCollision(Collidable* other, Vector2 penetration) override;

private:
	GridCoord lastCell;
	bool killedByPlayer;

	EnemyType type;
	int damage;
	float attackCooldown;
	float currentAttackCooldown;

	AnimatedSprite* moving;
	AnimatedSprite* attacking;
	AnimatedSprite* death;
};

#endif