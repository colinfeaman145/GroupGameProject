#include "Enemy.hpp"
#include "AnimatedSprite.hpp"
#include "GameContext.hpp"


Enemy::Enemy() {
	LoadEntityDataFromJson("Enemy");
}


Enemy::~Enemy() = default;

void Enemy::Initialize(Vector2 pos, float retarget, int targetRad, float atlasTarget, float playerTarget) {

    // the idle animation is always the base animation
    Attackable::Initialize(pos, idleAnimation);
    collideType = CollidableType::ENEMY;
    idleAnimation->Animate();
}

void Enemy::Draw(Renderer* renderer) {

    Attackable::Draw(renderer);
}

void Enemy::Process(float deltaTime) {

    //standard process
    Attackable::Process(deltaTime);
    context.grid->UpdateOccupancy((Entity*)this, &GridCell::AddOther, &GridCell::RemoveOther);
}

void Enemy::SetType(EnemyType t) {
    type = t;
}

EnemyType Enemy::GetType() {
    return type;
}

void Enemy::SetDamage(int d) {
    damage = d;
}

int Enemy::GetDamage() {
    return damage;
}



void Enemy::SetAttackCooldown(float atckCool) {
    attackCooldown = atckCool;
    currentAttackCooldown = attackCooldown;
}


float Enemy::GetAttackCooldown() {
    return attackCooldown;
}

void Enemy::SetKilledByPlayer() {
    killedByPlayer = true;
}


void Enemy::HandleCollision(Collidable* other, Vector2 penetration) {
    if (!IsAlive()) return;
    //Attackable::HandleCollision(other, penetration);
    if (other->GetCollidableType() == CollidableType::ENEMY) return;//dont attack own kind

}