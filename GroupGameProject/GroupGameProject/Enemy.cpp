#include "Enemy.hpp"
#include "AnimatedSprite.hpp"
#include "GameContext.hpp"
#include "StatSheet.hpp"
#include "InlineHelper.hpp"
#include "Grid.hpp"

Enemy::Enemy() = default;
Enemy::~Enemy() = default;

void Enemy::Initialize(Vector2 pos) {

	LoadEntityDataFromJson(data);
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

void Enemy::HandleCollision(Collidable* other, Vector2 penetration) {
    if (!IsAlive()) return;
    if (other->GetCollidableType() != CollidableType::PLAYER) return;//only damage player

	if (auto contact = dynamic_cast<Attackable*>(other)) {

		auto damage = m_pStats->GetFinalDamage();
		HitInfo info = {
			.damageDealt = damage,
		};
		DealDamageTo(contact, info);
	}

}

void Enemy::OnStuck() {
}

void Enemy::SetAttackCooldown(float atckCool) {
    attackCooldown = atckCool;
    currentAttackCooldown = attackCooldown;
}

void Enemy::ChangeState(EnemyState newState) {
    currentState = newState;
}


float Enemy::GetAttackCooldown() {
    return attackCooldown;
}
