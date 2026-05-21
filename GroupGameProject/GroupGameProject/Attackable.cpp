#include "Attackable.hpp"
#include "InlineHelper.hpp"

bool Attackable::Initialize(Vector2 pos, Sprite* spr) {
	Entity::Initialize(pos, spr);

	// default statsheet values
	m_pStats = new StatSheet();
	m_pStats->baseHealth = 100;
	m_pStats->bonusHealth = 0;
	m_pStats->healthMult = 1;

	m_pStats->baseSpeed = 1000;
	m_pStats-> bonusSpeed = 0;
	m_pStats->speedMult = 1;

	m_pStats->baseDamage = 100;
	m_pStats->bonusDamage = 0;
	m_pStats->damageMult = 1;

	m_fCurrentHealth = 100;
	return true;
}

void Attackable::Process(float deltaTime) {
	Entity::Process(deltaTime);

	if (m_fCurrentHealth < 0) isAlive = false;
	else if (m_fCurrentHealth > 0) isAlive = true;

	healthBar->SetPosition(position.x, position.y);

	if (flashDuration > 0) flashDuration -= deltaTime;
	else SetFlash(false);
}

void Attackable::Draw(Renderer* renderer) {
	healthBar->Draw(renderer);
}

int Attackable::GetHealth() {
	return m_fCurrentHealth;
}

int Attackable::GetMaxHealth() {
	if (m_pStats) {
		return m_pStats->GetHealth();
	} 
	return 0;
}

void Attackable::SetHealth(float h) {

	// clip health to 0 and max health
	int maxHealth = m_pStats ? m_pStats->GetHealth() : h;
	m_fCurrentHealth = clip(h, 0, m_pStats ? m_pStats->GetHealth() : h);

	Vector2 size = sprite->GetDrawSize();
	healthBar = new PercentageBar(m_fCurrentHealth, maxHealth, size.x * 0.9, size.y * 0.1, { 255, 50, 50, 150 }, { 0, 0, 0, 150 });
	healthBar->SetPosition(position.x, position.y);
	healthBar->SetOffset((size.x * 0.05), (size.y * 0.8));
}

void Attackable::ApplyDamage(float amount) {
	float maxHealth = m_pStats ? m_pStats->GetHealth() : m_fCurrentHealth;

	if (amount == -1) {//full kill
		m_fCurrentHealth = 0;
		return;
	}

	m_fCurrentHealth = clip(m_fCurrentHealth - amount, 0, maxHealth);
	SetFlash(true);
	healthBar->SetValues(m_fCurrentHealth, maxHealth);
}

void Attackable::Heal(float amount) {
	float maxHealth = m_pStats ? m_pStats->GetHealth() : m_fCurrentHealth;

	if (amount == -1) {//full heal
		m_fCurrentHealth = maxHealth;
		return;
	}

	m_fCurrentHealth = clip(m_fCurrentHealth + amount, 0, maxHealth);
	healthBar->SetValues(m_fCurrentHealth, maxHealth);
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