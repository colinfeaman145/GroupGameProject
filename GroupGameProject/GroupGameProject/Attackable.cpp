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
#include "AnimatedSprite.hpp"


Attackable::Attackable() {
	context.dc->RegisterOnLevelUp([this] {this->AddItem(7,1);});

	m_itemSpawner = new ItemSpawner();

	// register callback to automatically recalculate stats when inventory changes
	m_inventory = new Inventory();
	m_inventory->RegisterCallback([this]() { this->RecalculateStats();});

	// default statsheet values
	m_pStats = new StatSheet();
	m_pStats->SetDefaultValues();
	m_pStats->Reset();
}

Attackable::~Attackable() {
	delete idleAnimation;
	idleAnimation = nullptr;
	delete movingAnimation;
	movingAnimation = nullptr;
	delete deathAnimation;
	deathAnimation = nullptr;
	delete attackingAnimation;
	attackingAnimation = nullptr;
}

bool Attackable::Initialize(Vector2 pos, Sprite* spr) {
	Entity::Initialize(pos, spr);
	
	// healthbar setup
	Vector2 size = sprite->GetDrawSize();
	healthBar = new PercentageBar(m_pStats->GetCurrentHealth(), m_pStats ? m_pStats->GetFinalHealth() : m_pStats->GetCurrentHealth(), size.x * 1, size.y * 0.1, {255, 50, 50, 255}, {150, 50, 50, 255}, RenderLayer::PERCENTBAR);
	healthBar->SetPosition(position.x, position.y);
	healthBar->SetOffset(-(size.x * 0.05), (size.y * 0.2));

	isAlive = true;
	return true;
}

void Attackable::Process(float deltaTime) {

	if (!IsAlive() && !IsDying()) return;

	Entity::Process(deltaTime);

	healthBar->SetPosition(position.x, position.y);

	if (flashDuration > 0) flashDuration -= deltaTime;
	else SetFlash(false);

	TickStatusEffect(deltaTime);
	TickRegeneration(deltaTime);
	healthBar->SetValues(m_pStats->GetCurrentHealth(), m_pStats ? m_pStats->GetFinalHealth() : m_pStats->GetCurrentHealth());

}

void Attackable::Draw(Renderer* renderer) {

	if (!IsAlive() && !IsDying()) return;
	Entity::Draw(renderer);
	healthBar->Draw(renderer);
}

void Attackable::TickRegeneration(float deltaTime) {
	if (!m_pStats) return;
	if (m_pStats->regernation <= 0) return;

	auto healTickInSeconds = 0.5f;
	auto healAmount = m_pStats->regernation * healTickInSeconds; // because the tick is every 0.5 seconds

	HitInfo info = {
		.healAmount = healAmount,
		.isCritical = false
	};

	EventContext ctx = {
		.source = this,
		.target = this,
		.hitInfo = info
	};

	m_fLastHealTick += deltaTime;
	if (m_fLastHealTick < healTickInSeconds) return;// tick every half a second
	m_fLastHealTick = 0;
	ApplyHeal(ctx);

}

int Attackable::GetHealth() {
	return m_pStats->GetCurrentHealth();
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
	m_pStats->SetCurrentHealth(h);
	Vector2 size = sprite->GetDrawSize();
	healthBar = new PercentageBar(m_pStats->GetCurrentHealth(), maxHealth, size.x * 0.9, size.y * 0.1, {255, 50, 50, 150}, {0, 0, 0, 150}, RenderLayer::PERCENTBAR);
	healthBar->SetPosition(position.x, position.y);
	healthBar->SetOffset((size.x * 0.05), (size.y * 0.8));
}

// returns the actual damage received after armor calculations
void Attackable::ApplyDamage(EventContext& ctx) {
	float damageReceived = m_pStats ? m_pStats->CalculateDamageReceived(ctx.hitInfo.damageDealt) : ctx.hitInfo.damageDealt;
	ctx.hitInfo.damageDealt = damageReceived;

	if (ctx.hitInfo.damageDealt == -1) {//full kill
		m_pStats->SetCurrentHealth(0);
	}
	if (ctx.hitInfo.damageDealt == 0) { // should not proc event if 0 damage is done
		return;
	}

	m_pStats->SetCurrentHealth(m_pStats->GetCurrentHealth() - damageReceived);
	SetFlash(true);
	healthBar->SetValues(m_pStats->GetCurrentHealth(), m_pStats->GetFinalHealth());

	if (m_pStats->GetCurrentHealth() <= 0 && IsAlive()) {
		FireEvent(EventType::OnDeath, ctx);
		ctx.source->FireEvent(EventType::OnKill, ctx);
		SetDead();
	}

	FireEvent(EventType::OnGettigHit, ctx);
}

void Attackable::ApplyHeal(EventContext& ctx) {
	auto isHealCrit = HasHitChance(m_pStats->critChance) && (m_pStats->hasHealCritEnabled);

	if (ctx.hitInfo.healAmount == -1) {//full heal
		m_pStats->SetCurrentHealth(m_pStats->GetFinalHealth());
		return;
	}
	if (isHealCrit) {
		ctx.hitInfo.healAmount *= m_pStats->critMultiplyer;
		ctx.hitInfo.isCritical = true;
	}

	m_pStats->SetCurrentHealth(m_pStats->GetCurrentHealth() + ctx.hitInfo.healAmount);
	healthBar->SetValues(m_pStats->GetCurrentHealth(), m_pStats->GetFinalHealth());

	FireEvent(EventType::OnHeal, ctx);
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

// if this method is called it is assumed it has actaully hit the target so OnHit and OnGettingHit effects are procced
void Attackable::DealDamageTo(Attackable* target, HitInfo info) {
	if (HasHitChance(m_pStats->critChance)) {
		info.damageDealt *= m_pStats->critMultiplyer;
		info.isCritical = true;
	}

	EventContext ctx;
	ctx.source = this;
	ctx.target = target;
	ctx.hitInfo = info;

	target->ApplyDamage(ctx);

	FireEvent(EventType::OnHit, ctx);
	if (info.isCritical) {
		FireEvent(EventType::OnCrit, ctx);
	}
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


void Attackable::SetSprites(AnimatedSprite* move, AnimatedSprite* attack, AnimatedSprite* die) {
    movingAnimation = static_cast<AnimatedSprite*>(move->Clone());
    attackingAnimation = static_cast<AnimatedSprite*>(attack->Clone());
    deathAnimation = static_cast<AnimatedSprite*>(die->Clone());
    movingAnimation->Animate();
}

void Attackable::SetSpritesDrawSize(int size) {
    movingAnimation->SetDrawSize(size, size);
    attackingAnimation->SetDrawSize(size, size);
    deathAnimation->SetDrawSize(size, size);

    radius = size / 4;
    collisionBound = CollisionShape::MakeCircle(radius, Vector2(radius, radius));
    SetCanCollide(true);
}

void Attackable::SetSpriteDirection(bool b) {
    movingAnimation->SetFlip(b);
    attackingAnimation->SetFlip(b);
    deathAnimation->SetFlip(b);
}


void Attackable::SetDead() {
	// already dead
	if (!isAlive) return;
	isAlive = false;

    SetCanCollide(false);
	m_itemSpawner->SpawnItems(GetPosition());

	if (deathAnimation) {
		deathAnimation->Restart();
		deathAnimation->SetLooping(false);
		deathAnimation->Animate();
		deathAnimation->SetPosition(position.x, position.y);
		sprite = deathAnimation;
	}
}


int Attackable::GetItemCount(ItemID id) {
	return m_inventory->Count(id);
}

bool Attackable::IsDying() {
    if (isAlive) return false;

    bool deathPlaying = sprite == deathAnimation && deathAnimation->IsAnimating();
    return deathPlaying;
}

std::unordered_map<ItemID, int> Attackable::GetItems() {
	return m_inventory->All();
}

void Attackable::ApplyStatusEffect(StatusEffectType status, float duration, Attackable* source) {
	m_activeStatusEffects.push_back({status, duration, source});
}

void Attackable::TickStatusEffect(float deltaTime) {
	if (m_activeStatusEffects.empty()) return;
	if (!m_pStats) return;

	m_fLastStatusEffectTick += deltaTime;
	auto applyTick = m_fLastStatusEffectTick >= 0.5; // tick every half a second
	EventContext ctx;
	for (auto& status : m_activeStatusEffects) {

		// bleeding effect
		if (status.type == StatusEffectType::Bleeding) {

			ctx.source = status.source;
			ctx.target = this;
			ctx.hitInfo = { m_pStats->GetCurrentHealth() * 0.05f, false, false };
			status.duration -= deltaTime;
		}

		// burning effect
		if (status.type == StatusEffectType::Burning) {
			ctx.source = status.source;
			ctx.target = this;
			ctx.hitInfo = { 1, false, false };
			status.duration -= deltaTime;
		}
		// poison effect
		if (status.type == StatusEffectType::Poisoning) {
			ctx.source = status.source;
			ctx.target = this;
			ctx.hitInfo = { m_pStats->GetFinalHealth() * 0.05f, false, false };
			status.duration -= deltaTime;
		}
		// invincible effect
		if (status.type == StatusEffectType::Invincible) {
			
			ctx.source = status.source;
			ctx.target = this;
			ctx.hitInfo = { 0, false, false };
			SetCanCollide(false);
			status.duration -= deltaTime;
		}

		// only apply damage every tick
		if (applyTick) ApplyDamage(ctx);
	}
	if (applyTick) m_fLastStatusEffectTick = 0;

	std::erase_if(m_activeStatusEffects, [](const StatusEffect& s) { 
		if (s.duration <= 0) {
			// OnExit logic
			if (s.type == StatusEffectType::Invincible) {
				s.source->SetCanCollide(true);
			}
			return true;
		}
		return false; 
	});
}

void Attackable::AddItem(ItemID id, int count) {
	m_inventory->Add(id, count);
	for (auto& [itemID, stacks] : m_inventory->All()) {
		ItemDef def = context.ir->Get(itemID);
		if (def.effect) {
			def.effect->OnPickup(this, stacks);
		}
	}
}

void Attackable::RemoveItem(ItemID id, int count) {
	m_inventory->Remove(id, count);
	for (auto& [itemID, stacks] : m_inventory->All()) {
		ItemDef def = context.ir->Get(itemID);
		if (def.effect) {
			def.effect->OnRemove(this, stacks);
		}
	}
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

void Attackable::LoadEntityDataFromJson(json data) {
	this->data = data;
	LoadStatsFromJson(data["stats"]);
	LoadInventoryFromJson(data["inventory"]);
	LoadItemSpawnerSettingsFromJson(data["spawner"]);
	LoadAnimationsFromJson(data["animations"]);
	m_pStats->SetCurrentHealth(m_pStats->GetFinalHealth());// set current health after item calculations
}

void Attackable::LoadAnimationsFromJson(json animations) {
	//animation setup
	if (auto path = animations["idle"]["path"]; !path.get<std::string>().empty()) {
		SDL_Texture* idleTexture = context.txm->LoadTexture(context.renderer, path);
		idleAnimation = new AnimatedSprite();
		idleAnimation->Initialize(idleTexture, animations["idle"]["frameWidth"], animations["idle"]["frameHeight"], 0, 0, 500, 500, animations["idle"]["framesPerRow"], animations["idle"]["framesTotal"]);
		idleAnimation->SetDrawLayer(RenderLayer::ENEMIES);
		idleAnimation->SetFrameDuration(animations["idle"]["frameDuration"]);
		idleAnimation->SetLooping(true);
		idleAnimation->SetLeaveOnLastFrame(true);
	}


	if (auto path = animations["death"]["path"]; !path.get<std::string>().empty()) {
		SDL_Texture* deathTexture = context.txm->LoadTexture(context.renderer, path);
		deathAnimation = new AnimatedSprite();
		deathAnimation->Initialize(deathTexture, animations["death"]["frameWidth"], animations["death"]["frameHeight"], 0, 0, 500, 500, animations["death"]["framesPerRow"], animations["death"]["framesTotal"]);
		deathAnimation->SetDrawLayer(RenderLayer::ENEMIES);
		deathAnimation->SetFrameDuration(animations["death"]["frameDuration"]);
		deathAnimation->SetLooping(true);
		deathAnimation->SetLeaveOnLastFrame(true);
	}


	if (auto path = animations["attacking"]["path"]; !path.get<std::string>().empty()) {
		SDL_Texture* attackTexture = context.txm->LoadTexture(context.renderer, path);
		attackingAnimation = new AnimatedSprite();
		attackingAnimation->Initialize(attackTexture, animations["attacking"]["frameWidth"], animations["attacking"]["frameHeight"], 0, 0, 500, 500, animations["attacking"]["framesPerRow"], animations["attacking"]["framesTotal"]);
		attackingAnimation->SetDrawLayer(RenderLayer::ENEMIES);
		attackingAnimation->SetFrameDuration(animations["attacking"]["frameDuration"]);
		attackingAnimation->SetLooping(true);
		attackingAnimation->SetLeaveOnLastFrame(true);
	}


	if (auto path = animations["moving"]["path"]; !path.get<std::string>().empty()) {
		SDL_Texture* movingTexture = context.txm->LoadTexture(context.renderer, path);
		movingAnimation = new AnimatedSprite();
		movingAnimation->Initialize(movingTexture, animations["moving"]["frameWidth"], animations["moving"]["frameHeight"], 0, 0, 500, 500, animations["moving"]["framesPerRow"], animations["moving"]["framesTotal"]);
		movingAnimation->SetDrawLayer(RenderLayer::ENEMIES);
		movingAnimation->SetFrameDuration(animations["moving"]["frameDuration"]);
		movingAnimation->SetLooping(true);
		movingAnimation->SetLeaveOnLastFrame(true);
	}
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
		stats["baseAttackSpeed"].get<float>(),
		stats["bonusAttackSpeed"].get<float>(),
		stats["attackSpeedMult"].get<float>(),
		stats["armor"].get<int>(),
		stats["regeneration"].get<float>(),
		stats["critChance"].get<float>(),
		stats["critMultiplyer"].get<float>(),
		stats["hasHealCritEnabled"].get<int>()
	);
	m_pStats->Reset();
}