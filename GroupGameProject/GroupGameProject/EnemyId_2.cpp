#include "EnemyId_2.hpp"
#include <math.h>
#include "InlineHelper.hpp"
#include "AnimatedSprite.hpp"
#include "GameContext.hpp"


void EnemyId_2::Initialize(Vector2 pos) {
	Enemy::Initialize(pos);
	ChangeState(EnemyState::IDLE);
	acceleration = data["params"]["acceleration"];
	deceleration = data["params"]["deceleration"];
	maxSpeed = data["params"]["maxSpeed"];
}


void EnemyId_2::Process(float deltaTime) {
	Enemy::Process(deltaTime);
	if (target == nullptr) return;
	if (currentState == EnemyState::IDLE) {
		RecalculateDirection();
		ChangeState(EnemyState::APPROACHING);
	}

	if (GetPosition().IsNear(currentTargetPosition, GetRadius())) {
		ChangeState(EnemyState::RECOVERING);
	}

	if (velocity.IsNear({ 0,0 }, 5) && currentState == EnemyState::RECOVERING) {
		ChangeState(EnemyState::IDLE);
	}

	if (abs(velocity.x) < maxSpeed && abs(velocity.y) < maxSpeed) {
		Approach(deltaTime);
	}
	Recover(deltaTime);
}




// attack player while in a specific animation
void EnemyId_2::Approach(float deltaTime) {
	if (currentState != EnemyState::APPROACHING) return;
	velocity.x *= 1 + (acceleration * deltaTime);
	velocity.y *= 1 + (acceleration * deltaTime);
	HandleAttackAnimation();
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

// if not hit the player
void EnemyId_2::Recover(float deltaTime) {
	if (currentState != EnemyState::RECOVERING) return;
	velocity.x /= 1 + (deceleration * deltaTime);
	velocity.y /= 1 + (deceleration * deltaTime);
	HandleRecoverAnimation();
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

void EnemyId_2::RecalculateDirection() {
	currentTargetPosition = target->GetPosition();
	velocity = (currentTargetPosition - GetPosition()).Normalized();
}




