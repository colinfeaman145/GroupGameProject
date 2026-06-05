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
    SetCollisionBound(CollisionShape::MakeCircle(radius * 0.75));
    idleAnimation->Animate();
}

void Enemy::Draw(Renderer* renderer) {

    Attackable::Draw(renderer);
}

void Enemy::Process(float deltaTime) {

    //standard process
    Attackable::Process(deltaTime);
    context.grid->UpdateOccupancy((Entity*)this, &GridCell::AddOther, &GridCell::RemoveOther);

    //if player nearby KILL FUCK EM UP
    if (target == nullptr) {
        GridCoord myCoord = context.grid->WorldToGrid(GetPosition());
        for (Collidable* other : context.grid->GetNearbyCollidables(myCoord, 7)) {
            if (!other) continue;
            if (other->GetCollidableType() == CollidableType::PLAYER) {
                SetTarget(other);
                break;
            }
        }
    }

    AI::Process(deltaTime);
}

void Enemy::HandleCollision(Collidable* other, Vector2 penetration) {
    Attackable::HandleCollision(other, penetration);
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
