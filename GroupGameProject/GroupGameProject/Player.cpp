#include "Player.hpp"
#include <SDL.h>
#include "AnimatedSprite.hpp"
#include "GameContext.hpp"
#include "StatSheet.hpp"
#include "PercentageBar.hpp"
#include "Camera.hpp"
#include "ItemSpawner.hpp"

void Player::Initialize(Vector2 pos, AnimatedSprite* spr) {
	Attackable::Initialize(pos, spr);

	LoadEntityDataFromJson("Player");


    SDL_Texture* playerRunning = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Soldier/soldier_walk.png");
    moving = new AnimatedSprite();
    moving->Initialize(playerRunning, 34, 34, 0, 0, 500, 500, 2, 4);
    moving->SetDrawLayer(RenderLayer::PLAYER);
    moving->SetFrameDuration(0.25);
    moving->SetLooping(true);
    moving->SetLeaveOnLastFrame(true);

	collideType = CollidableType::PLAYER;

	// set initial idle animation
	idle = spr;
}

void Player::Process(float deltaTime) {
	Attackable::Process(deltaTime);

	auto occ = GetOccupancy();


	HandleMouseClick();
	HandleMovement();
	HandleAnimation();
	healthBar->SetValues(m_fCurrentHealth, m_pStats ? m_pStats->GetFinalHealth() : m_fCurrentHealth);
}

void Player::HandleAnimation() {
	if (velocity.x != 0 || velocity.y != 0) {
		if (sprite != moving) {
			moving->Animate();
			sprite = moving;
		}
	}
	else {
		if (sprite != idle) {
			idle->Animate();
			sprite = idle;
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

		// activate spawner on mouse click
		m_itemSpawner->SpawnItems(mousePos);
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


void Player::SetSprites(AnimatedSprite* move, AnimatedSprite* idle) {
	moving = static_cast<AnimatedSprite*>(move->Clone());
	idle = static_cast<AnimatedSprite*>(idle->Clone());
	idle->Animate();
}
