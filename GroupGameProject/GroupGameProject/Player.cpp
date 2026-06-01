#include "Player.hpp"
#include <SDL.h>
#include "AnimatedSprite.hpp"
#include "GameContext.hpp"
#include "StatSheet.hpp"
#include "PercentageBar.hpp"
#include "Camera.hpp"
#include "ItemSpawner.hpp"
#include "Inventory.hpp"



Player::Player() {
	LoadEntityDataFromJson("Player");
}

void Player::Initialize(Vector2 pos) {
	Attackable::Initialize(pos, idleAnimation);

	collideType = CollidableType::PLAYER;
	dodgeCooldown = 1;
	dodgeDuration = 0.5;
	dodgeDistance = 2;

	idleAnimation->Animate();
}

void Player::Process(float deltaTime) {
	Attackable::Process(deltaTime);

	auto occ = GetOccupancy();
	dodgeTimer -= deltaTime;
	dodgeCooldownTimer -= deltaTime;

	HandleMouseClick(deltaTime);
	HandleMovement();
	HandleAnimation();
	healthBar->SetValues(m_fCurrentHealth, m_pStats ? m_pStats->GetFinalHealth() : m_fCurrentHealth);

    //collision updates
    context.grid->UpdateOccupancy((Entity*)this, &GridCell::AddOther, &GridCell::RemoveOther);

	if (attackCooldown > 0) {
		attackCooldown -= deltaTime;
	}
}

void Player::HandleAnimation() {
	if (velocity.x != 0 || velocity.y != 0) {
		if (sprite != movingAnimation) {
			movingAnimation->Animate();
			sprite = movingAnimation;
		}
	}
	else {
		if (sprite != idleAnimation) {
			idleAnimation->Animate();
			sprite = idleAnimation;
		}
	}
	sprite->SetFlip(velocity.x > 0);//flip if moving right
}

void Player::HandleMouseClick(float deltaTime) {
	// left click actions just for testing
	if (context.im->IsMouseButtonDown(1)) {
		if (attackCooldown > 0) return;
		attackCooldown = 1 / m_pStats->GetFinalAttackSpeed();
		auto mousePos = context.im->GetMouseWorldPosition(context.renderer->cam);
		auto hitInfo = HitInfo{ 
			.damageDealt = m_pStats ? m_pStats->GetFinalDamage() : 0,
			.isCritical = false,
			.isDodged = false 
		};
		auto event = EventContext{ 
			.source = this,
			.targetPosition = mousePos,
			.hitInfo = hitInfo
		};

		// executes all onAttack item effects from the inventory
		FireEvent(EventType::OnAttack, event);
	}
	if (context.im->IsMouseButtonPressed(3)) {
		auto event = EventContext{ 
			.source = this,
		};
		FireEvent(EventType::OnCashout, event);
	}
}

void Player::HandleMovement() {

	if (dodgeTimer > 0) {//no inputs while dodgeing
		return;
	}
	else if (dodging) {
		dodging = false;
		velocity = Vector2();
		movingAnimation->SetRotation(0);
	}

	// horizontal movement
	if (context.im->IsKeyDown("move_left")) {
		velocity.x = -m_pStats->GetFinalSpeed();
		dodgeDirection.x = velocity.x;
	}
	else if (context.im->IsKeyDown("move_right")) {
		velocity.x = m_pStats->GetFinalSpeed();
		dodgeDirection.x = velocity.x;
	}
	else {
		velocity.x = 0;
	}

	// vertical movement
	if (context.im->IsKeyDown("move_up")) {
		velocity.y = -m_pStats->GetFinalSpeed();
		dodgeDirection.y = velocity.y;
	}
	else if (context.im->IsKeyDown("move_down")) {
		velocity.y = m_pStats->GetFinalSpeed();
		dodgeDirection.y = velocity.y;
	}
	else {
		velocity.y = 0;
	}

	// normalize diagonal movement so speed is consistent in all directions
	if (velocity.x != 0 && velocity.y != 0) {
		velocity = velocity.Normalized() * m_pStats->GetFinalSpeed();
	}

	//roll/dodge
	if (context.im->IsMouseButtonDown(3) || context.im->IsKeyDown("dodge")) {
		if (dodgeCooldownTimer > 0) return;

		Vector2 dir;
		if (velocity.x == 0 && velocity.y == 0) {
			dir = dodgeDirection;
		}
		else {
			dir = velocity;
		}
		dir = dir.Normalized() * m_pStats->GetFinalSpeed() * dodgeDistance;
		velocity = dir;

		movingAnimation->Spin(dodgeDuration);
		dodgeTimer = dodgeDuration;
		dodgeCooldownTimer = dodgeCooldown;
		dodging = true;
		ApplyStatusEffect(StatusEffectType::Invincible, dodgeDuration * 1.1, this);
	}


}

void Player::Draw(Renderer* renderer) {
 	Attackable::Draw(renderer);
	renderer->cam->Follow(GetPosition());
	healthBar->Draw(renderer);
}

void Player::HandleCollision(Collidable* other, Vector2 penetration) {
	//handle collisions here
}


