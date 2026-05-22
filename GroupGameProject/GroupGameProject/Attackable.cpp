#include "Attackable.hpp"
#include "InlineHelper.hpp"

bool Attackable::Initialize(Vector2 pos, Sprite* spr) {
	Entity::Initialize(pos, spr);

	// register callback to automatically recalculate stats when inventory changes
	inventory = new Inventory(
		[this]() { this->RecalculateStats(); }
	);

	// default statsheet values
	m_pStats = new StatSheet();
	m_pStats->Reset();

	m_fCurrentHealth = m_pStats->GetFinalHealth();
	return true;
}

void Attackable::Process(float deltaTime) {
	Entity::Process(deltaTime);

	if (m_fCurrentHealth < 0) isAlive = false;
	else if (m_fCurrentHealth > 0) isAlive = true;

	healthBar->SetPosition(position.x, position.y);

	if (flashDuration > 0) flashDuration -= deltaTime;
	else SetFlash(false);

	ApplyHeal(m_pStats ? m_pStats->regernation * deltaTime : 0);
}

void Attackable::Draw(Renderer* renderer) {
	healthBar->Draw(renderer);
}

int Attackable::GetHealth() {
	return m_fCurrentHealth;
}

int Attackable::GetMaxHealth() {
	if (m_pStats) {
		return m_pStats->GetFinalHealth();
	} 
	return 0;
}

void Attackable::SetHealth(float h) {

	// clip health to 0 and max health
	int maxHealth = m_pStats ? m_pStats->GetFinalHealth() : h;
	m_fCurrentHealth = clip(h, 0, m_pStats ? m_pStats->GetFinalHealth() : h);

	Vector2 size = sprite->GetDrawSize();
	healthBar = new PercentageBar(m_fCurrentHealth, maxHealth, size.x * 0.9, size.y * 0.1, { 255, 50, 50, 150 }, { 0, 0, 0, 150 });
	healthBar->SetPosition(position.x, position.y);
	healthBar->SetOffset((size.x * 0.05), (size.y * 0.8));
}

// returns the actual damage received after armor calculations
float Attackable::ApplyDamage(HitInfo info) {
	float maxHealth = m_pStats ? m_pStats->GetFinalHealth() : m_fCurrentHealth;
	float damageReceived = m_pStats ? m_pStats->CalculateDamageReceived(info.damageDealt) : info.damageDealt;

	if (info.damageDealt == -1) {//full kill
		m_fCurrentHealth = 0;
		return -1;
	}

	m_fCurrentHealth = clip(m_fCurrentHealth - damageReceived, 0, maxHealth);
	SetFlash(true);
	healthBar->SetValues(m_fCurrentHealth, maxHealth);
	return damageReceived;
}

void Attackable::ApplyHeal(float amount) {
	float maxHealth = m_pStats ? m_pStats->GetFinalHealth() : m_fCurrentHealth;

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


void Attackable::DealDamageTo(Attackable* target, HitInfo info) {
	float damageDealt = target->ApplyDamage(info);

	EventContext ctx;
	ctx.source = this;
	ctx.target = target;
	ctx.hitInfo = info;

	FireEvent(EventType::OnHit, ctx);
	if (info.isCritical) {
		FireEvent(EventType::OnCrit, ctx);
	}
	target->FireEvent(EventType::OnGettigHit, ctx);
}

// apply effects of items on event
void Attackable::FireEvent(EventType type, EventContext ctx) {
	// fire item effects
	for (auto& [itemID, stacks] : inventory->All()) {
		ItemDef def = context.ir->Get(itemID);
		if (def.effect) {
			def.effect->OnEvent(type, ctx, stacks);
		}
	}
}

void Attackable::ApplyStatusEffect(StatusEffectType status, Attackable* source) {
	m_activeStatusEffects.push_back({status, 3.0f, source});
}

void Attackable::TickStatusEffect(float deltaTime) {
	if (m_activeStatusEffects.empty()) return;
	if (!m_pStats) return;
	for (auto& status : m_activeStatusEffects) {

		// bleeding effect
		if (status.type == StatusEffectType::Burning) {
			ApplyDamage({ 5 * deltaTime, false, false }); //apply 5 damage per second
			status.duration -= deltaTime;
		}
		// burning effect
		if (status.type == StatusEffectType::Burning) {
			ApplyDamage({ m_pStats->GetFinalHealth() * 0.1f * deltaTime, false, false });// 10% max health damage per second
			status.duration -= deltaTime;
		}
		
		
	}
	std::erase_if(m_activeStatusEffects, [](const StatusEffect& s) { return s.duration <= 0; });

}

void Attackable::RecalculateStats() {
	if (!m_pStats) return;
	m_pStats->Reset();

	for (auto& [itemID, stacks] : inventory->All()) {
		ItemDef def = context.ir->Get(itemID);
		if (def.effect) {
			def.effect->OnModifyStats(*m_pStats, stacks);
		}
	}
}