#include "EnemyId_2.hpp"
#include <math.h>
#include "InlineHelper.hpp"
#include "AnimatedSprite.hpp"
#include "GameContext.hpp"
#include "StatSheet.hpp"


bool EnemyId_2::Initialize(Vector2 pos, Sprite* spr) {
	Enemy::Initialize(pos);
	ChangeState(EnemyState::IDLE);

	return true;
}

void EnemyId_2::Process(float deltaTime) {
    Enemy::Process(deltaTime);
    if (!IsAlive() || target == nullptr) return;

    UpdateState();
    sprite->SetFlip(target->GetPosition().x < GetPosition().x);
}

void EnemyId_2::UpdateState() {
    // Don't interrupt a playing attack animation
    if (currentState == EnemyState::ATTACKING && attackingAnimation->IsAnimating())
        return;

    EnemyState newState = IsTargetInAttackRange() ? EnemyState::ATTACKING
        : EnemyState::APPROACHING;
    if (newState != currentState)
        EnterState(newState);
}

void EnemyId_2::EnterState(EnemyState newState) {
    currentState = newState;

    switch (newState) {

    case EnemyState::APPROACHING:
        sprite = idleAnimation;
        idleAnimation->Animate();
        attackingAnimation->Restart();
        attackingAnimation->Pause();
        isChasing = true;
        break;

    case EnemyState::ATTACKING:
        sprite = attackingAnimation;
        attackingAnimation->Restart();
        attackingAnimation->Animate();
        idleAnimation->Pause();
        isChasing = false;
        velocity = { 0, 0 };
        DoAttack();
        break;
    }
}

void EnemyId_2::DoAttack() {
    if (auto player = dynamic_cast<Attackable*>(target))
        player->ApplyStatusEffect({ StatusEffectType::Bleeding, 5.0f, 0, 0, this });
}

void EnemyId_2::HandleCollision(Collidable* other, Vector2 penetration) {
	Enemy::HandleCollision(other, penetration);
}
