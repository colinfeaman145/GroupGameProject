#include "EnemyId_3.hpp"
#include "StatSheet.hpp"
#include "AnimatedSprite.hpp"
#include "GameContext.hpp"
#include "InlineHelper.hpp"
#include "PercentageBar.hpp"

bool EnemyId_3::Initialize(Vector2 pos, Sprite* spr) {
	Enemy::Initialize(pos);
    int size = context.grid->GetCellSize() * 2;
    radius *= 0.75;
    movingAnimation->SetDrawSize(size, size);
    idleAnimation->SetDrawSize(size, size);
    attackingAnimation->SetDrawSize(size, size);
    SetCollisionBound(CollisionShape::MakeCircle(radius, Vector2((size / 2) - radius, (size / 2) - radius)));
    healthBar->SetOffset((size - healthBar->GetWidth()) / 2, size * 0.2);
	return true;
}

void EnemyId_3::Draw(Renderer* renderer) {
	Enemy::Draw(renderer);
}

void EnemyId_3::Process(float deltaTime) {
    Enemy::Process(deltaTime);

    if (attackCooldown > 0)
        attackCooldown -= deltaTime;

    UpdateState();
    sprite->SetFlip(velocity.x < 0);
}

void EnemyId_3::UpdateState() {
    if (!IsAlive() || target == nullptr) return;

    // dont interrupt attacking animation
    if (currentState == EnemyState::ATTACKING && attackingAnimation->IsAnimating())
        return;

    bool inRange = IsTargetInAttackRange();
    bool onCooldown = attackCooldown > 0;

    EnemyState newState;
    if (!inRange)
        newState = EnemyState::APPROACHING;
    else if (onCooldown)
        newState = EnemyState::IDLE;
    else
        newState = EnemyState::ATTACKING;



	//printf("EnemyId_3 State: %d - %d\n", newState, isChasing);
    if (newState != currentState)
        EnterState(newState);
}

void EnemyId_3::HandleCollision(Collidable* other, Vector2 penetration) {
	Enemy::HandleCollision(other, penetration);
}

void EnemyId_3::EnterState(EnemyState newState) {
    currentState = newState;

    switch (newState) {

    case EnemyState::APPROACHING:
        sprite = movingAnimation;
        movingAnimation->Restart();
        movingAnimation->Animate();
        idleAnimation->Pause();
        attackingAnimation->Pause();
        isChasing = true;
        break;

    case EnemyState::IDLE:
        sprite = idleAnimation;  // or an idle animation if you have one
        idleAnimation->Restart();
		idleAnimation->Animate();
        movingAnimation->Pause();
        attackingAnimation->Pause();
        velocity = { 0, 0 };
        isChasing = false;
        break;

    case EnemyState::ATTACKING:
        sprite = attackingAnimation;
        attackingAnimation->Restart();
        attackingAnimation->Animate();
        attackingAnimation->SetLooping(false);
		idleAnimation->Pause();
        movingAnimation->Pause();
        velocity = { 0, 0 };
        isChasing = false;
        DoAttack();
        break;
    }
}

void EnemyId_3::DoAttack() {
    attackCooldown = 1.f / m_pStats->GetFinalAttackSpeed();

    auto hitInfo = HitInfo{
        .damageDealt = m_pStats ? m_pStats->GetFinalDamage() : 0,
        .isCritical = false,
        .isDodged = false
    };
    auto ctx = EventContext{
        .source = this,
        .targetPosition = target->GetPosition(),
        .hitInfo = hitInfo
    };

    FireEvent(EventType::OnAttack, ctx);
}
