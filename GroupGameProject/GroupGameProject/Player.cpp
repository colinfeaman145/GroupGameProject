#include "Player.hpp"

void Player::Initialize(Vector2 pos, AnimatedSprite* spr) {
	Attackable::Initialize(pos, spr);


	//animation setup
    SDL_Texture* playerIdle = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Soldier/soldier_idle.png");
    idle = new AnimatedSprite();
    idle->Initialize(playerIdle, 34, 34, 0, 0, 500, 500, 3, 5);
    idle->SetDrawLayer(RenderLayer::PLAYER);
    idle->SetFrameDuration(0.25);
    idle->SetLooping(true);
    idle->SetLeaveOnLastFrame(true);
	sprite = idle;
	idle->Animate();

    SDL_Texture* playerRunning = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Soldier/soldier_walk.png");
    moving = new AnimatedSprite();
    moving->Initialize(playerRunning, 34, 34, 0, 0, 500, 500, 2, 4);
    moving->SetDrawLayer(RenderLayer::PLAYER);
    moving->SetFrameDuration(0.25);
    moving->SetLooping(true);
    moving->SetLeaveOnLastFrame(true);

	
	// grid setup
	Vector2 size = sprite->GetDrawSize();
	healthBar = new PercentageBar(m_fCurrentHealth, m_pStats ? m_pStats->GetHealth() : m_fCurrentHealth, size.x * 1.1, size.y * 0.2, {255, 50, 50}, {150, 50, 50});
	healthBar->SetPosition(position.x, position.y);
	healthBar->SetOffset(-(size.x * 0.05), (size.y * 0.7));


}

void Player::Process(float deltaTime) {
	Attackable::Process(deltaTime);

	HandleMovement();
	HandleAnimation();
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

void Player::HandleMovement() {
	if (context.im->IsKeyDown("move_left")) {
		velocity.x = -m_pStats->GetSpeed();
	}
	else if (context.im->IsKeyDown("move_right")) {
		velocity.x = m_pStats->GetSpeed();
	}
	else {
		velocity.x = 0;
	}

	if (context.im->IsKeyDown("move_up")) {
		velocity.y = -m_pStats->GetSpeed();
	}
	else if (context.im->IsKeyDown("move_down")) {
		velocity.y = m_pStats->GetSpeed();
	}
	else {
		velocity.y = 0;
	}
}

void Player::Draw(Renderer* renderer) {
	Entity::Draw(renderer);
	renderer->cam->Follow(GetPosition());
}

void Player::HandleCollision(Collidable* other, Vector2 penetration) {
	//handle collisions here
}


void Player::SetSprites(AnimatedSprite* move, AnimatedSprite* idle) {
	moving = static_cast<AnimatedSprite*>(move->Clone());
	idle = static_cast<AnimatedSprite*>(idle->Clone());
	idle->Animate();
}
