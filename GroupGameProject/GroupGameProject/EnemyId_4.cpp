#include "EnemyId_4.hpp"
#include "GameContext.hpp"
#include "StatSheet.hpp"
#include "AnimatedSprite.hpp"
#include "Bullet.hpp"
#include "PercentageBar.hpp"

EnemyId_4::EnemyId_4(): lastAttack(-1), bulletHellWavesLeft(0), bulletHellWaveTimer(0) {

}

bool EnemyId_4::Initialize(Vector2 pos, Sprite* spr) {

    Enemy::Initialize(pos);
    int size = context.grid->GetCellSize() * 3;
    radius *= 0.75;
    movingAnimation->SetDrawSize(size, size);
    idleAnimation->SetDrawSize(size, size);
    attackingAnimation->SetDrawSize(size, size);
    SetCollisionBound(CollisionShape::MakeCircle(radius, Vector2((size / 2) - radius, (size / 2) - radius)));
    healthBar->SetOffset((size - healthBar->GetWidth()) / 2, size * 0.3);

    EnterState(BossState::APPROACHING);

    chargeSpeed = data["params"]["chargeSpeed"];
    bulletSpeed = data["params"]["bulletSpeed"];
    bulletHellWaveCount = data["params"]["bulletWaveCount"];
    bulletHellWaveInterval = data["params"]["bulletWaveInterval"];
    return true;
}

void EnemyId_4::Process(float deltaTime) {
    Enemy::Process(deltaTime);
    if (!IsAlive() || target == nullptr) return;

    if (attackCooldown > 0)
        attackCooldown -= deltaTime;

    switch (currentState) {
    case BossState::APPROACHING: TickApproaching();  break;
    case BossState::ATTACKING:   TickAttacking(deltaTime);    break;
    case BossState::CHARGING:    TickCharging(deltaTime); break;
    case BossState::COOLDOWN:    TickCooldown();     break;
    }

    sprite->SetFlip(velocity.x < 0);
}


void EnemyId_4::TickApproaching() {
    if (!IsTargetInAttackRange()) return;
    isChasing = false;
    velocity = { 0, 0 };
    PickAndStartNextAttack();
}

void EnemyId_4::TickAttacking(float deltaTime) {

    // handle bullet hell waves
    if (bulletHellWavesLeft > 0) {
        bulletHellWaveTimer -= deltaTime ; // need deltaTime here — see below
        if (bulletHellWaveTimer <= 0.f) {
            FireBulletHellWave();
            bulletHellWavesLeft--;
            bulletHellWaveTimer = bulletHellWaveInterval;
        }
        return; // don't exit ATTACKING until all waves are done
    }
    // wait for animation to finish
    if (attackingAnimation->IsAnimating()) return;
    EnterState(BossState::COOLDOWN);

    // wait for animation to finish

    if (attackingAnimation->IsAnimating()) return;
    EnterState(BossState::COOLDOWN);
}

void EnemyId_4::TickCharging(float deltaTime) {
    GridCoord next = context.grid->WorldToGrid(GetPosition() + velocity.Normalized() * radius);
    GridCell* cell = context.grid->GetCell(next);
    if (cell && cell->IsWall()) {
        OnChargeHitWall();
        return;
    }

}

void EnemyId_4::TickCooldown() {
    if (attackCooldown > 0) return;
    EnterState(BossState::APPROACHING);
}

void EnemyId_4::PickAndStartNextAttack() {
    int next;
    next = rand() % 2;
    lastAttack = next;

    switch (next) {
    case 0: StartBulletHell();  break;
    case 1: StartCharge();      break;
    }
}

// Attack 1: Bullet hell 
void EnemyId_4::StartBulletHell() {
    EnterState(BossState::ATTACKING);
    bulletHellWavesLeft = bulletHellWaveCount;
    bulletHellWaveTimer = 0.f; // fire first wave immediately
}

void EnemyId_4::FireBulletHellWave() {
    constexpr int   bulletCount = 16;
    constexpr float angleStep = (2.f * 3.14159f) / bulletCount;

    // rotate each wave slightly so they don't all overlap
    float waveOffset = (bulletHellWaveCount - bulletHellWavesLeft) * (angleStep * 0.5f);

    for (int i = 0; i < bulletCount; i++) {
        float angle = i * angleStep + waveOffset;
        Vector2 targetPos = GetCenter() + Vector2{ cosf(angle), sinf(angle) } * 1000.f;
        SpawnBullet(targetPos, false);
    }
}

// Attack 2: Charge 
void EnemyId_4::StartCharge() {
    EnterState(BossState::CHARGING);

    Vector2 toTarget = (target->GetPosition() - GetPosition()).Normalized();
    velocity = toTarget * chargeSpeed;
    isChasing = false;
}

void EnemyId_4::OnChargeHitWall() {
    velocity = { 0, 0 };
    EnterState(BossState::COOLDOWN);
}


void EnemyId_4::SpawnBullet(Vector2 targetPos, bool explosive) {
    Bullet* bullet = new Bullet("../../assets/sprites/Bullets/AR_SR_DMR_Machinegun_bullet.png");

    auto hitInfo = HitInfo{
        .damageDealt = m_pStats ? m_pStats->GetFinalDamage() : 0,
        .isCritical = false,
        .isDodged = false
    };
    auto ctx = EventContext{
        .source = this,
        .targetPosition = targetPos,
        .hitInfo = hitInfo
    };

	bullet->Initialize(ctx, 1, 5.f, bulletSpeed, atan2f(targetPos.y - GetPosition().y, targetPos.x - GetPosition().x));
	context.grid->UpdateOccupancy((Entity*)bullet, &GridCell::AddOther, &GridCell::RemoveOther);
}


void EnemyId_4::EnterState(BossState newState) {
    currentState = newState;

    switch (newState) {
    case BossState::APPROACHING:
        sprite = movingAnimation;
        movingAnimation->Animate();
        idleAnimation->Pause();
        attackingAnimation->Pause();
        isChasing = true;
        break;

    case BossState::ATTACKING:
        sprite = attackingAnimation;
        attackingAnimation->Restart();
        attackingAnimation->Animate();
        attackingAnimation->SetLooping(false);
        idleAnimation->Pause();
        movingAnimation->Pause();
        break;

    case BossState::CHARGING:
        sprite = movingAnimation;
        movingAnimation->Animate();
        idleAnimation->Pause();
        attackingAnimation->Pause();
        break;

    case BossState::COOLDOWN:
        sprite = idleAnimation;
        idleAnimation->Restart();
        idleAnimation->Animate();
        movingAnimation->Pause();
        attackingAnimation->Pause();
        velocity = { 0, 0 };
        attackCooldown = 1.f / m_pStats->GetFinalAttackSpeed();
        break;
    }
}
