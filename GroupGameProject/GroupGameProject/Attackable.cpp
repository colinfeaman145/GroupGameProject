#include "Attackable.hpp"

void Attackable::Process(float deltaTime) {

	if (health < 0) isAlive = false;
	else if (health > 0) isAlive = true;

	healthBar->SetPosition(position.x, position.y);

	if (flashDuration > 0) flashDuration -= deltaTime;
	else SetFlash(false);
}

void Attackable::Draw(Renderer* renderer) {
	healthBar->Draw(renderer);
}

int Attackable::GetHealth() {
	return health;
}

int Attackable::GetMaxHealth() {
	return maxHealth;
}

void Attackable::SetHealth(float h) {
	maxHealth = h;
	health = h;

	Vector2 size = sprite->GetDrawSize();
	healthBar = new PercentageBar(health, maxHealth, size.x * 0.9, size.y * 0.1, { 255, 50, 50, 150 }, { 0, 0, 0, 150 });
	healthBar->SetPosition(position.x, position.y);
	healthBar->SetOffset((size.x * 0.05), (size.y * 0.8));
}

void Attackable::Damage(float amount) {
	if (amount == -1) {//full kill
		health = 0;
		return;
	}

	health -= amount;
	SetFlash(true);
	healthBar->SetValues(health, maxHealth);
}

void Attackable::Heal(float amount) {
	if (amount == -1) {//full heal
		health = maxHealth;
		return;
	}

	health += amount;
	if (health >= maxHealth)
		health = maxHealth;
	healthBar->SetValues(health, maxHealth);
}

void Attackable::SetPosition(Vector2 pos) {
	Entity::SetPosition(pos);
	healthBar->SetPosition(pos.x, pos.y);
}

void Attackable::SetFlash(bool flash) {
	if (!sprite) return;
	sprite->SetIsFlashing(flash);
	if (flash) flashDuration = 0.25f;
}