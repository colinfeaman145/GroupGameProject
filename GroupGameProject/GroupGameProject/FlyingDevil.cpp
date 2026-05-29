#include "FlyingDevil.hpp"
#include <math.h>
#include "InlineHelper.hpp"
#include "AnimatedSprite.hpp"


void FlyingDevil::Initialize(Vector2 pos) {
	LoadEntityDataFromJson("FlyingDevil");
	Enemy::Initialize(pos);
	ChangeState(EnemyState::IDLE);
	acceleration = params["acceleration"];
	deceleration = params["deceleration"];
	maxSpeed = params["maxSpeed"];
}


void FlyingDevil::Process(float deltaTime) {
	Enemy::Process(deltaTime);
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
void FlyingDevil::Approach(float deltaTime) {
	if (currentState != EnemyState::APPROACHING) return;
	velocity.x *= 1 + (acceleration * deltaTime);
	velocity.y *= 1 + (acceleration * deltaTime);
	HandleAttackAnimation();
}

void FlyingDevil::HandleAttackAnimation() {
	if (!attackingAnimation->IsAnimating()) {
		sprite = attackingAnimation;
		attackingAnimation->Animate();
		idleAnimation->Restart();
		idleAnimation->Pause();
		if (target->GetPosition().x < GetPosition().x) sprite->SetFlip(true);
		else sprite->SetFlip(false);
	}

}

void FlyingDevil::HandleCollision(Collidable* other, Vector2 penetration) {
	Enemy::HandleCollision(other, penetration);
	if (other->GetCollidableType() != CollidableType::PLAYER) return;
	if (auto player = dynamic_cast<Attackable*>(other)) {
		player->ApplyStatusEffect(StatusEffectType::Bleeding, 5.0f, this);
	}
}

// if not hit the player
void FlyingDevil::Recover(float deltaTime) {
	if (currentState != EnemyState::RECOVERING) return;
	velocity.x /= 1 + (deceleration * deltaTime);
	velocity.y /= 1 + (deceleration * deltaTime);
	HandleRecoverAnimation();
}

void FlyingDevil::HandleRecoverAnimation() {
	if (!idleAnimation->IsAnimating()) {
		sprite = idleAnimation;
		idleAnimation->Animate();
		attackingAnimation->Restart();
		attackingAnimation->Pause();

		if (target->GetPosition().x < GetPosition().x) sprite->SetFlip(true);
		else sprite->SetFlip(false);
	}
}

void FlyingDevil::RecalculateDirection() {
	currentTargetPosition = target->GetPosition();
	velocity = (currentTargetPosition - GetPosition()).Normalized();
}




