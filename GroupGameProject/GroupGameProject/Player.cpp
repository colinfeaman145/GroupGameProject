#include "Player.hpp"
#include <SDL.h>
#include "AnimatedSprite.hpp"
#include "GameContext.hpp"
#include "StatSheet.hpp"
#include "PercentageBar.hpp"
#include "Camera.hpp"
#include "ItemSpawner.hpp"



Player::Player() {
	LoadEntityDataFromJson("Player");
}

void Player::Initialize(Vector2 pos) {
	Attackable::Initialize(pos, idleAnimation);


	collideType = CollidableType::PLAYER;

	idleAnimation->Animate();
}

void Player::Process(float deltaTime) {
	Attackable::Process(deltaTime);

	auto occ = GetOccupancy();


	HandleMouseClick();
	HandleMovement();
	HandleAnimation();
	healthBar->SetValues(m_fCurrentHealth, m_pStats ? m_pStats->GetFinalHealth() : m_fCurrentHealth);

    //collision updates
    context.grid->UpdateOccupancy((Entity*)this, &GridCell::AddOther, &GridCell::RemoveOther);
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
	sprite->SetFlip(velocity.x > 0);//flip if moving left
}

void Player::HandleMouseClick() {
	// left click actions just for testing
	if (context.im->IsMouseButtonPressed(1)) {
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

	// just very basic movement handling
	// improve it if you want

	// horizontal movement
	if (context.im->IsKeyDown("move_left")) {
		velocity.x = -m_pStats->GetFinalSpeed();
	}
	else if (context.im->IsKeyDown("move_right")) {
		velocity.x = m_pStats->GetFinalSpeed();
	}
	else {
		velocity.x = 0;
	}

	// vertical movement
	if (context.im->IsKeyDown("move_up")) {
		velocity.y = -m_pStats->GetFinalSpeed();
	}
	else if (context.im->IsKeyDown("move_down")) {
		velocity.y = m_pStats->GetFinalSpeed();
	}
	else {
		velocity.y = 0;
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

