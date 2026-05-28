#include "Enemy.hpp"
#include "AnimatedSprite.hpp"
#include "GameContext.hpp"
#include "StatSheet.hpp"
#include "InlineHelper.hpp"


Enemy::Enemy() = default;
Enemy::~Enemy() = default;

void Enemy::Initialize(Vector2 pos) {

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
    if (other->GetCollidableType() == CollidableType::ENEMY) return;//dont attack own kind

    if (auto contact = dynamic_cast<Attackable*>(other)) {
        if (contact->GetCollidableType() == CollidableType::PLAYER) {

            auto damage = m_pStats->GetFinalDamage();
            HitInfo info = {
                .damageDealt = damage,
            };
            DealDamageTo(contact, info);
        }
    }

}

void Enemy::OnStuck() {
}

void Enemy::SetAttackCooldown(float atckCool) {
    attackCooldown = atckCool;
    currentAttackCooldown = attackCooldown;
}


float Enemy::GetAttackCooldown() {
    return attackCooldown;
}
