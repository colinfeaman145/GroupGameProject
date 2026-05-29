#include "EnemyId_3.hpp"
#include "StatSheet.hpp"
#include "AnimatedSprite.hpp"
#include "GameContext.hpp"
#include "InlineHelper.hpp"

void EnemyId_3::Initialize(Vector2 pos) {
	Enemy::Initialize(pos);
	RecalculateTargetLocation();
}

void EnemyId_3::Draw(Renderer* renderer) {
	Enemy::Draw(renderer);
}

void EnemyId_3::Process(float deltaTime) {
	Enemy::Process(deltaTime);


	HandleAttack();


	

	if (GetPosition().IsNear(currentTargetPos, GetRadius())) {
		RecalculateTargetLocation();
	}

	if (attackCooldown > 0) {
		attackCooldown -= deltaTime;
	}
}

void EnemyId_3::RecalculateTargetLocation() {
	auto newYPos = GetRandomIntrBetween(0, context.grid->GetGridHeight() * context.grid->GetCellSize());
	auto newXPos = GetRandomIntrBetween(0, context.grid->GetGridWidth() * context.grid->GetCellSize());

	currentTargetPos = Vector2{ (float)newXPos, (float)newYPos }  ;

	velocity = (currentTargetPos - GetPosition()).Normalized() * m_pStats->GetFinalSpeed();

}

void EnemyId_3::HandleAttack() {
	if (!IsAlive()) return;
	if (attackCooldown > 0 ) {
		// play moving animatin if attackanimation is finished - moving animatin should not be started every frame
		if (attackingAnimation->IsAnimating() == false && movingAnimation->IsAnimating() == false) {
			sprite = movingAnimation;
			attackingAnimation->Pause();
			movingAnimation->Restart();
			movingAnimation->Animate();
			RecalculateTargetLocation();
		}

		return;
	}

	
	sprite = attackingAnimation;
	movingAnimation->Pause();
	attackingAnimation->Restart();
	attackingAnimation->Animate();
	attackingAnimation->SetLooping(false);
	velocity = { 0,0 };

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

void EnemyId_3::HandleCollision(Collidable* other, Vector2 penetration) {
	Enemy::HandleCollision(other, penetration);
}
