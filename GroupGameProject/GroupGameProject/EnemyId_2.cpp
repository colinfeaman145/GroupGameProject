#include "EnemyId_2.hpp"
#include <math.h>
#include "InlineHelper.hpp"
#include "AnimatedSprite.hpp"
#include "GameContext.hpp"
#include "StatSheet.hpp"


bool EnemyId_2::Initialize(Vector2 pos, Sprite* spr) {
	Enemy::Initialize(pos);
	ChangeState(EnemyState::IDLE);
	moveSpeed = m_pStats->GetFinalSpeed();
	acceleration = data["params"]["acceleration"];
	deceleration = data["params"]["deceleration"];
	maxSpeed = data["params"]["maxSpeed"];
	targetRadius = data["params"]["targetRadius"];
	return true;
}

void EnemyId_2::Process(float deltaTime) {
	Enemy::Process(deltaTime);
	if (target == nullptr) return;
	
	if ((target->GetPosition() - GetPosition()).Length() > targetRadius) {
		HandleRecoverAnimation();
		isChasing = false;
	}
	else {
		HandleAttackAnimation();
		isChasing = true;
	}
}

void EnemyId_2::HandleAttackAnimation() {
	if (!attackingAnimation->IsAnimating()) {
		sprite = attackingAnimation;
		attackingAnimation->Animate();
		idleAnimation->Restart();
		idleAnimation->Pause();
		if (target->GetPosition().x < GetPosition().x) sprite->SetFlip(true);
		else sprite->SetFlip(false);
	}
}

void EnemyId_2::HandleCollision(Collidable* other, Vector2 penetration) {
	Enemy::HandleCollision(other, penetration);
	if (other->GetCollidableType() != CollidableType::PLAYER) return;
	if (auto player = dynamic_cast<Attackable*>(other)) {
		player->ApplyStatusEffect({ StatusEffectType::Bleeding, 5.0f, 0, 0,this });
	}
}

void EnemyId_2::HandleRecoverAnimation() {
	if (!idleAnimation->IsAnimating()) {
		sprite = idleAnimation;
		idleAnimation->Animate();
		attackingAnimation->Restart();
		attackingAnimation->Pause();

	}
	if (target->GetPosition().x < GetPosition().x) sprite->SetFlip(true);
	else sprite->SetFlip(false);
}

