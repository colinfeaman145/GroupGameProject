// BossEnemy.hpp
#pragma once
#include "Enemy.hpp"

enum class BossState { 
    APPROACHING,
    ATTACKING,
    CHARGING, 
    COOLDOWN
};

class EnemyId_4 : public Enemy {
public:
	EnemyId_4();
    bool Initialize(Vector2 pos, Sprite* spr) override;
    void Process(float deltaTime) override;

private:
    BossState currentState = BossState::APPROACHING;
    int lastAttack;
    int bulletHellWavesLeft;
    float bulletHellWaveTimer;

    float attackRange;
    float chargeSpeed;
    float bulletSpeed;
    int bulletHellWaveCount;
    float bulletHellWaveInterval;

    void EnterState(BossState newState);
    void PickAndStartNextAttack();

    void TickApproaching();
    void TickAttacking(float deltaTime);
    void TickCharging(float deltaTime);
    void TickCooldown();

    void StartBulletHell();
    void FireBulletHellWave();
    void StartCharge();
    void OnChargeHitWall();

    void SpawnBullet(Vector2 direction, bool explosive);
};

