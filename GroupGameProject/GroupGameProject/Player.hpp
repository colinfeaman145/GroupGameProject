#pragma once
#include "Attackable.hpp"

class PlayerHUD;
class AnimatedSprite;
class Player : public Attackable {

public:
	~Player() override;
	void Initialize(Vector2 pos);
	void Process(float deltaTime) override;
	void Draw(Renderer* renderer) override;
	void HandleCollision(Collidable* other, Vector2 penetration) override;

private:
	void HandleAnimation();
	void HandleMouseClick(float deltaTime);
	void HandleMovement();


private:
	int coinCount;
	float attackCooldown;


	float dodgeDuration; //how long dodge lasts
	float dodgeTimer; //currently dodgeing timer
	float dodgeCooldown; //time between dodges
	float dodgeCooldownTimer;
	float dodgeDistance;
	Vector2 dodgeDirection;
	bool dodging;

	PlayerHUD* playerHud;

};

