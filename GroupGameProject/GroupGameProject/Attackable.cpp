#include "Attackable.hpp"

#include <fstream>

#include "InlineHelper.hpp"
#include "Sprite.hpp"
#include "Inventory.hpp"
#include "StatSheet.hpp"
#include "PercentageBar.hpp"
#include "ItemRegistry.hpp"
#include "GameContext.hpp"
#include "ItemSpawner.hpp"


bool Attackable::Initialize(Vector2 pos, Sprite* spr) {
	Entity::Initialize(pos, spr);
	m_itemSpawner = new ItemSpawner();

	// register callback to automatically recalculate stats when inventory changes
	m_inventory = new Inventory(
		[this]() { this->RecalculateStats(); }
	);

	// default statsheet values
	m_pStats = new StatSheet();
	m_pStats->SetDefaultValues();
	m_pStats->Reset();
	m_fCurrentHealth = m_pStats->GetFinalHealth();

	isAlive = true;

	// healthbar setup
	Vector2 size = sprite->GetDrawSize();
	healthBar = new PercentageBar(m_fCurrentHealth, m_pStats ? m_pStats->GetFinalHealth() : m_fCurrentHealth, size.x * 1, size.y * 0.1, {255, 50, 50, 255}, {150, 50, 50, 255});
	healthBar->SetPosition(position.x, position.y);
	healthBar->SetOffset(-(size.x * 0.05), (size.y * 0.2));

	return true;
}

void Attackable::Process(float deltaTime) {
	Entity::Process(deltaTime);

	if (m_fCurrentHealth < 0) isAlive = false;
	else if (m_fCurrentHealth > 0) isAlive = true;

	healthBar->SetPosition(position.x, position.y);

	if (flashDuration > 0) flashDuration -= deltaTime;
	else SetFlash(false);

	TickStatusEffect(deltaTime);
	TickRegeneration(deltaTime);
}

void Attackable::TickRegeneration(float deltaTime) {
	if (!m_pStats) return;
	if (m_pStats->regernation <= 0) return;

	m_fLastHealTick += deltaTime;
	if (m_fLastHealTick < 0.5) return;// tick every half a second
	m_fLastHealTick = 0;
	ApplyHeal(m_pStats->regernation);

}

void Attackable::Draw(Renderer* renderer) {
	Entity::Draw(renderer);
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
	info.damageDealt = damageDealt;

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
	for (auto& [itemID, stacks] : m_inventory->All()) {
		ItemDef def = context.ir->Get(itemID);
		if (def.effect) {
			def.effect->OnEvent(type, ctx, stacks);
		}
	}
}

void Attackable::ApplyStatusEffect(StatusEffectType status, Attackable* source) {
	m_activeStatusEffects.push_back({status, 5.0f, source});
}

void Attackable::TickStatusEffect(float deltaTime) {
	if (m_activeStatusEffects.empty()) return;
	if (!m_pStats) return;

	m_fLastStatusEffectTick += deltaTime;
	if (m_fLastStatusEffectTick < 0.5) return;// tick every half a second

	for (auto& status : m_activeStatusEffects) {

		// bleeding effect
		if (status.type == StatusEffectType::Bleeding) {
			ApplyDamage({ m_fCurrentHealth * 0.05f, false, false }); //apply 5% current health damage per tick
			status.duration -= m_fLastStatusEffectTick;
		}

		// bleeding effect
		if (status.type == StatusEffectType::Burning) {
			ApplyDamage({  1, false, false }); //apply 1 damage per tick
			status.duration -= m_fLastStatusEffectTick;
		}
		// burning effect
		if (status.type == StatusEffectType::Poisoning) {
			ApplyDamage({ m_pStats->GetFinalHealth() * 0.05f, false, false });// 5% max health damage per tick
			status.duration -= m_fLastStatusEffectTick;
		}
		
	}
	std::erase_if(m_activeStatusEffects, [](const StatusEffect& s) { return s.duration <= 0; });
	m_fLastStatusEffectTick = 0;
}

void Attackable::AddItem(ItemID id, int count) {
	m_inventory->Add(id, count);
}

void Attackable::RecalculateStats() {
	if (!m_pStats) return;
	m_pStats->Reset();

	for (auto& [itemID, stacks] : m_inventory->All()) {
		ItemDef def = context.ir->Get(itemID);
		if (def.effect) {
			def.effect->OnModifyStats(*m_pStats, stacks);
		}
	}
}

void Attackable::LoadEntityDataFromJson(const string& section) {
	auto filepath = "../../data/entities.json";
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Failed to open stats file: " << filepath << endl;
        return;
    }
    json data = json::parse(file);
	LoadStatsFromJson(data[section]["stats"]);
	LoadInventoryFromJson(data[section]["inventory"]);
	LoadItemSpawnerSettingsFromJson(data[section]["spawner"]);
	m_fCurrentHealth = m_pStats->GetFinalHealth();// set current health after item calculations
}


void Attackable::LoadInventoryFromJson(json items) {
	m_inventory->Clear();
	for (auto& item : items) {
		int itemId = item["id"].get<int>();
		int stacks = item["stacks"].get<int>();
		m_inventory->Add(itemId, stacks);
	}
}

void Attackable::LoadItemSpawnerSettingsFromJson(json config) {
	m_itemSpawner->Initialise(config);
}

void Attackable::LoadStatsFromJson(json stats) {
	if (!m_pStats) return;
	m_pStats->SetDefaultValues(
		stats["baseHealth"].get<int>(),
		stats["bonusHealth"].get<int>(),
		stats["healthMult"].get<float>(),
		stats["baseDamage"].get<int>(),
		stats["bonusDamage"].get<int>(),
		stats["damageMult"].get<float>(),
		stats["baseSpeed"].get<int>(),
		stats["bonusSpeed"].get<int>(),
		stats["speedMult"].get<float>(),
		stats["armor"].get<int>(),
		stats["regeneration"].get<float>()
	);
	m_pStats->Reset();
}