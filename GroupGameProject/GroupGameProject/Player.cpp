#include "Player.hpp"
#include <SDL.h>
#include "AnimatedSprite.hpp"
#include "InlineHelper.hpp"
#include "GameContext.hpp"
#include "StatSheet.hpp"
#include "PercentageBar.hpp"
#include "Camera.hpp"
#include "ItemSpawner.hpp"
#include "Inventory.hpp"
#include "Grid.hpp"
#include "Prop.hpp"
#include "PlayerHUD.hpp"


Player::~Player() {
	delete playerHud;
	playerHud = nullptr;
}

void Player::Initialize(Vector2 pos) {
	LoadEntityDataFromJson(data);
	Attackable::Initialize(pos, idleAnimation);

    //player hud
	if (playerHud == nullptr) {
		playerHud = new PlayerHUD(this);
		playerHud->Initialize();
	}

	int playerWidth = sprite->GetWidth() * 0.3;
	int playerHeight = sprite->GetHeight() * 0.45;
	SetCollisionBound(CollisionShape::MakeAABB(playerWidth, playerHeight, Vector2(-playerWidth / 2, 0)));
	collideType = CollidableType::PLAYER;
	dodgeCooldown = 1;
	dodgeDuration = 0.5;
	dodgeDistance = 2;
	walkSoundTimer = 0.0f;

	idleAnimation->Animate();
}

void Player::Process(float deltaTime) {
	Attackable::Process(deltaTime);

	playerHud->Process(deltaTime);

	auto occ = GetOccupancy();
	dodgeTimer -= deltaTime;
	dodgeCooldownTimer -= deltaTime;

	HandleMouseClick(deltaTime);
	HandleMovement();
	HandleAnimation();

	if (attackCooldown > 0) {
		attackCooldown -= deltaTime;
	}

	if (velocity.x != 0 || velocity.y != 0) {
		walkSoundTimer -= deltaTime;
		if (walkSoundTimer <= 0) {
			FMOD_VECTOR pos = { GetPosition().x, 0, GetPosition().y };
			FMOD_VECTOR vel = { 0,0,0 };
			context.am->PlaySound("walking", "Footsteps", pos, vel, { 0.9f, 1.1f });			walkSoundTimer = 0.6f;
		}
	}
	else {
		walkSoundTimer = 0.6f;  // reset so first step plays immediately next time
	}

	context.grid->UpdateOccupancy((Entity*)this, &GridCell::AddOther, &GridCell::RemoveOther);

	//refresh flow field
	refreshFlowFieldTimer -= deltaTime;
	if (refreshFlowFieldTimer <= 0.f) {
		refreshFlowFieldTimer = 1.0f;
		GridCoord myCoord = context.grid->WorldToGrid(GetPosition());
		context.grid->InvalidateFlowFieldsNear(myCoord, 20);
	}
}

void Player::Draw(Renderer* renderer) {
 	Attackable::Draw(renderer);
	playerHud->Draw(renderer);
	renderer->cam->Follow(GetPosition());
	healthBar->Draw(renderer);
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

		FMOD_VECTOR pos = { GetPosition().x, 0, GetPosition().y };
		FMOD_VECTOR vel = { 0,0,0 };
		context.am->PlaySound("shoot_1", "SFX", pos, vel, { 0.85f, 1.15f });

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
		FMOD_VECTOR pos = { GetPosition().x, 0, GetPosition().y };
		FMOD_VECTOR vel = { 0,0,0 };
		context.am->PlaySound("dash", "SFX", pos, vel, { 0.9f, 1.1f });

		StatusEffect effect;
		effect.type = StatusEffectType::Invincible;
		effect.duration = dodgeDuration * 1.1;
		ApplyStatusEffect(effect);

		FireEvent(EventType::OnDodge, { .source = this, .target = this });
	}


}

void Player::HandleCollision(Collidable* other, Vector2 penetration) {
	Attackable::HandleCollision(other, penetration);
	//handle collisions here
	auto prop = dynamic_cast<Prop*>(other);
	if (prop != nullptr && prop->name == "Door") {
		if (m_inventory->Count(8) == 0) return;//need at least 1 key to open door
		context.dc->StageCompleted();
		m_inventory->Remove(8, 1);//remove 1 key
	}
}

void Player::AddItem(ItemID id, int count) {
	Attackable::AddItem(id, count);
	auto def = context.ir->Get(id);
	if (def.data.contains("name") && def.data.contains("description")) {
		playerHud->PushPopup(def.data["name"], def.data["description"]);
	}
}



