#pragma once
#include "ItemEffect.hpp"
#include "Attackable.hpp"
#include "StatSheet.hpp"
#include "AnimatedSprite.hpp"
#include "Bullet.hpp"
#include "Explosion.hpp"
#include "InlineHelper.hpp"
#include "GameContext.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Grid.hpp"
#include <algorithm>
#include <cmath>

// item 1 - Gun
// OnAttack fire a bullet towards the target location
class ItemId_1 : public ItemEffect {

	void OnEvent(EventType type, EventContext ctx, int stacks) {
		if (type != EventType::OnAttack) {
			return;
		}

		auto newBullet = new Bullet(data["params"]["bulletSprite"]);
		auto pierceCount = data["params"]["pierceCount"].get<float>();
		auto ttl = data["params"]["ttl"].get<float>();
		auto bulletSpeed = ItemEffect::GetLinearStackingItemValue(data["params"]["bulletSpeed"].get<float>(), data["params"]["bulletSpeedPerStack"].get<float>(), stacks);

        auto bulletRotation = atan2(ctx.targetPosition.y - ctx.source->GetPosition().y, ctx.targetPosition.x - ctx.source->GetPosition().x) * 180 / PI;
		newBullet->Initialize(ctx, pierceCount, ttl, bulletSpeed, bulletRotation);
		context.grid->UpdateOccupancy((Entity*)newBullet, &GridCell::AddOther, &GridCell::RemoveOther);
	}
};


// item 2 - BisonSteak
// Gain max health. 
class ItemId_2 : public ItemEffect {
	void OnPickup(Attackable* owner, int stacks) {
		auto baseHealth = data["params"]["baseHealth"].get<float>();
		auto increasePerStack = data["params"]["increasePerStack"].get<float>();
		float amount = stacks > 1 ? increasePerStack : baseHealth;
		owner->m_pStats->bonusHealth += amount;
		owner->m_pStats->currentHealth += amount;
	}

	void OnRemove(Attackable* owner, int stacks) {
		auto baseHealth = data["params"]["baseHealth"].get<float>();
		auto increasePerStack = data["params"]["increasePerStack"].get<float>();
		float amount = stacks > 1 ? increasePerStack : baseHealth;
		owner->m_pStats->bonusHealth -= amount;
		owner->m_pStats->currentHealth -= amount;
	}
};

// item 3 - Coin
// used as currency, just has to exist as a item
class ItemId_3 : public ItemEffect {}; 


// item 4 - CashoutModule
// controls the cashout logic, on kill gain a stack, on cashout give player random items based on current stack and remove stacks
class ItemId_4 : public ItemEffect {
	void OnEvent(EventType type, EventContext ctx, int stacks) {
		if (type == EventType::OnKill) {
			ctx.source->AddItem(data["id"], 1);
		}

		if (type == EventType::OnCashout) {
			auto count = ctx.source->GetItemCount(data["id"]);

			// find current tier
			auto currentTier = 0;
			json currentTierConfig;
			auto currentKillCount = ctx.source->GetItemCount(data["id"]) - 1;
			for (auto item : data["params"]["cashoutTiers"]) {
				if (currentKillCount < item["threshold"]) break;
				currentTierConfig = item;
				currentTier++;
			}
			if (currentTier == 0) return;

			// generate items for the player depending on item tier chance of current tier
			std::vector<ItemDef> itemsToSpawn;
			std::uniform_real_distribution<float> dist(0.0f, 1.0f);
			auto itemCountForPlayer = currentTierConfig["randomItemCount"].get<int>() + currentTierConfig["selectableItemCount"].get<int>();
			for (int i = 0; i < itemCountForPlayer; i++) {
				auto chanceHit = dist(gen);
				if (chanceHit <= currentTierConfig["legendaryChance"]) {
					itemsToSpawn.push_back(context.ir->GetRandomItemOfTier(ItemTier::Legendary));
					continue;
				}
				if (chanceHit <= currentTierConfig["epicChance"]) {
					itemsToSpawn.push_back(context.ir->GetRandomItemOfTier(ItemTier::Epic));
					continue;
				}
				if (chanceHit <= currentTierConfig["rareChance"]) {
					itemsToSpawn.push_back(context.ir->GetRandomItemOfTier(ItemTier::Rare));
					continue;
				}
				itemsToSpawn.push_back(context.ir->GetRandomItemOfTier(ItemTier::Common));
			}


			// present items to player (currently all random)
			for (auto item : itemsToSpawn) {
				if (item.id == 0) continue;
				auto newItem = new Item(item.data);
				newItem->SetBasePos(ctx.source->GetPosition());
				context.grid->UpdateOccupancy((Entity*)newItem, &GridCell::AddOther, &GridCell::RemoveOther);
			}

			ctx.source->RemoveItem(4, 999999); // remove all (hopefully)
			ctx.source->AddItem(4, 1);
		}
	}

};



// item 5 - InvincibleModule
// makes the owner invincible for a short amout of time
// prevents the system to apply damage to the player every frame
class ItemId_5 : public ItemEffect {
	void OnEvent(EventType type, EventContext ctx, int stacks) {
		if (type != EventType::OnGettingHit) return;
		ctx.target->ApplyStatusEffect({ StatusEffectType::Invincible, data["params"]["duration"], ctx.target });
	}
};
 
// item 6 - Sugar candy
// flat increase of attackspeed
class ItemId_6 : public ItemEffect {
	void OnPickup(Attackable* owner, int stacks) {
		auto increasePerStack = data["params"]["increasePerStack"].get<float>();
		owner->m_pStats->attackSpeedMult += increasePerStack;
	}

	void OnRemove(Attackable* owner, int stacks) {
		auto increasePerStack = data["params"]["increasePerStack"].get<float>();
		owner->m_pStats->attackSpeedMult -= increasePerStack;
	}
};

// item 7 - ScalingModule
// applies the scaling effect to a entity
// the stacks of this item are detemined by the progress in the current run
class ItemId_7 : public ItemEffect {
	void OnPickup(Attackable* owner, int stacks) {
		float healthIncrease = data["params"]["healthIncreasePerLevel"];
		float damageIncrease = data["params"]["damageIncreasePerLevel"];
		float healthPercent = owner->GetHealthPercent();
		owner->m_pStats->healthMult += healthIncrease;
		owner->m_pStats->damageMult += damageIncrease;
		owner->m_pStats->currentHealth = healthPercent * owner->m_pStats->GetFinalHealth();
	}

	void OnRemove(Attackable* owner, int stacks) {
		float healthIncrease = data["params"]["healthIncreasePerLevel"];
		float damageIncrease = data["params"]["damageIncreasePerLevel"];
		float healthPercent = owner->GetHealthPercent();
		owner->m_pStats->healthMult -= healthIncrease;
		owner->m_pStats->damageMult -= damageIncrease;
		owner->m_pStats->currentHealth = healthPercent * owner->m_pStats->GetFinalHealth();
	}
};


// item 8 - BossKey
// just has to exist as a item
// opens the door behind the boss
class ItemId_8 : public ItemEffect {};


// item 9 - ThornBracelet
// On getting hit, reflect a portion of the damage back to the attacker.
class ItemId_9 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnGettingHit) return;
		if (!ctx.source || ctx.hitInfo.damageDealt <= 0.f) return;

        float reflectPercent = GetHyperbolicStackingItemValue((float)data["params"]["reflectPercentage"], stacks);
        float reflectDamage = ctx.hitInfo.damageDealt * reflectPercent;
        ctx.target->DealDamageTo(ctx.source, {
            .damageDealt = reflectDamage,
            .healAmount = 0,
            .isCritical = false,
            .isDodged = false,
            .appliesOnHitEffects = true
        });
    }
};

// item 10 - DemonsEye
// Passive: crit chance per stack.
class ItemId_10 : public ItemEffect {
public:
	void OnPickup(Attackable* owner, int stacks) override {
		owner->m_pStats->critChance += (float)data["params"]["critChancePerStack"];
	}

	void OnRemove(Attackable* owner, int stacks) override {
		owner->m_pStats->critChance -= (float)data["params"]["critChancePerStack"];
	}
};

// item 11 - DemonsBlood
// On kill, heal for a certain amount.
class ItemId_11 : public ItemEffect {
public:
    void OnEvent(EventType type, EventContext ctx, int stacks) override {
        if (type != EventType::OnKill) return;
        float healAmt = GetLinearStackingItemValue(
            (float)data["params"]["baseHeal"],
            (int)data["params"]["healPerStack"],
            stacks);

		EventContext newCtx = {
			.source = ctx.source,
			.target = ctx.source,
			.targetPosition = ctx.source->GetPosition(),
			.hitInfo = {0, healAmt, false, false}
		};
        ctx.source->ApplyHeal(newCtx);
    }
};

// item 12 - WeightedBoots
// Passive: move speed per stack.
class ItemId_12 : public ItemEffect {
public:
	void OnPickup(Attackable* owner, int stacks) override {
		owner->m_pStats->bonusSpeed += owner->m_pStats->baseSpeed * (float)data["params"]["speedPerStack"];
	}

	void OnRemove(Attackable* owner, int stacks) override {
		owner->m_pStats->bonusSpeed -= owner->m_pStats->baseSpeed * (float)data["params"]["speedPerStack"];
	}
};

// item 13 - BerserkersHelmet
// On kill, gain a temporary attack speed boost.
class ItemId_13 : public ItemEffect {
public:
    void OnEvent(EventType type, EventContext ctx, int stacks) override {
        if (type != EventType::OnKill) return;

        float duration = (float)data["params"]["duration"];
        float boost = (float)data["params"]["attackSpeedBoostPerStack"] * stacks;

        ctx.source->ApplyStatusEffect({ StatusEffectType::AttackSpeedBoost, duration, ctx.source, boost });
    }
};

// item 14 - BloodyDagger
// On hit, chance to apply bleeding.
class ItemId_14 : public ItemEffect {
public:
    void OnEvent(EventType type, EventContext ctx, int stacks) override {
        if (type != EventType::OnHit) return;
        if (!ctx.target) return;

		//roll for bleed
		float chance = GetLinearStackingItemValue(
			(float)data["params"]["baseChance"],
			(int)data["params"]["chanceIncreasePerStack"],
			stacks);
		uniform_real_distribution<float> bleedRoll(0, 1);
		if (bleedRoll(gen) > chance) return;

		//apply bleeding
        float duration = GetLinearStackingItemValue(
            (float)data["params"]["baseDuration"],
            (int)data["params"]["durationPerStack"],
            stacks);
        ctx.target->ApplyStatusEffect({ StatusEffectType::Bleeding, duration, ctx.source });
    }
};

// item 15 - LooseAmunition
// // onHit have a chance to multiply the bullets and shoot at additional nearby targets 
class ItemId_15 : public ItemEffect {
public:
    void OnEvent(EventType type, EventContext ctx, int stacks) override {
        if (type != EventType::OnHit) return;

		auto chance = (float)data["params"]["procChance"] * stacks;
		auto targetNumBase = (int)data["params"]["targetNumBase"];
		auto targetNumIncreasePerStack = (int)data["params"]["targetNumIncreasePerStack"];
		auto baseRangeInTiles = (int)data["params"]["baseRangeInTiles"];
		auto rangePerStack = (int)data["params"]["rangePerStack"];

		auto finalTargetCount = ItemEffect::GetLinearStackingItemValue(targetNumBase,targetNumIncreasePerStack, stacks);
		auto range = ItemEffect::GetLinearStackingItemValue(baseRangeInTiles, rangePerStack, stacks);

		float dmg = ctx.hitInfo.damageDealt * (int)data["params"]["damageOfTotalDamage"];

		if (!HasHitChance(chance)) return;
		for (int i = 0; i < finalTargetCount; i++) {
			auto newBullet = new Bullet(data["params"]["bulletSprite"]);
			auto ttl = data["params"]["ttl"].get<float>();
			auto bulletSpeed = data["params"]["bulletSpeed"].get<int>();
			auto newTarget = context.grid->GetRandomEnemyInRange(ctx.target, range);

			if (newTarget == nullptr || !newTarget->IsAlive()) continue;
			ctx.targetPosition = ctx.target->GetPosition();
			ctx.target = newTarget;

			auto bulletRotation = atan2(newTarget->GetPosition().y - ctx.targetPosition.y, newTarget->GetPosition().x - ctx.targetPosition.x) * 180 / PI;
			newBullet->Initialize(ctx, 1, ttl, bulletSpeed, bulletRotation);
			context.grid->UpdateOccupancy((Entity*)newBullet, &GridCell::AddOther, &GridCell::RemoveOther);
		}
    }    
};

// item 16 - FireLordsBraces
// On kill, explode the corpse in an AoE.
class ItemId_16 : public ItemEffect {
public:
    void OnEvent(EventType type, EventContext ctx, int stacks) override {
        if (type != EventType::OnKill) return;

        float baseMult = (float)data["params"]["baseDamageMultiplier"];
        float stackMult = (float)data["params"]["damageMultiplierPerStack"];
        float damageMultiplier = ItemEffect::GetLinearStackingItemValue(baseMult, stackMult, stacks);

        float baseRad = (float)data["params"]["baseRadius"];
        float stackRad = (float)data["params"]["radiusPerStack"];
        float radius = ctx.target->GetRadius() * (baseRad + stackRad * stacks);

		bool targetIsPlayer = dynamic_cast<Player*>(ctx.target) != nullptr;

		ctx.hitInfo.damageDealt = ctx.source->m_pStats->baseDamage * damageMultiplier;
		ctx.targetPosition = ctx.target->GetPosition();

		Explosion* explosion = new Explosion(radius, ctx.hitInfo.damageDealt, targetIsPlayer);
		context.grid->UpdateOccupancy((Entity*)explosion, &GridCell::AddOther, &GridCell::RemoveOther);
    }
};

// item 17 - ExplosiveAmmunition
// On crit, set the target on fire 
class ItemId_17 : public ItemEffect {
public:
    void OnEvent(EventType type, EventContext ctx, int stacks) override {
        if (type != EventType::OnCrit) return;
        if (!ctx.target) return;

        float burnStrength = (float)data["params"]["burnDamageMultiplier"] * stacks;
        float burnDuration = (float)data["params"]["burnDuration"];

        ctx.target->ApplyStatusEffect({ StatusEffectType::Burning, burnDuration, ctx.source, burnStrength });
    }
};

// item 18 - PhantomCloak
// On dodge, gain invincibility and a damage bonus for a short window.
class ItemId_18 : public ItemEffect {
public:
    void OnEvent(EventType type, EventContext ctx, int stacks) override {
        if (type != EventType::OnDodge) return;

        float invDuration = GetLinearStackingItemValue(
            (float)data["params"]["baseInvincibleDuration"],
            (float)data["params"]["invincibleDurationPerStack"],
            stacks);

        float dmgBoost = (float)data["params"]["damageBoostPerStack"] * stacks;
        float dmgBoostDuration = invDuration + (float)data["params"]["boostDurationBonus"];

        ctx.source->ApplyStatusEffect({ StatusEffectType::Invincible, invDuration, ctx.source });
        ctx.source->ApplyStatusEffect({ StatusEffectType::DamageBoost, dmgBoostDuration, ctx.source, dmgBoost});
    }
};


// item 19 - DemonsHeart
// On kill, permanently increase max HP
class ItemId_19 : public ItemEffect {
public:
    void OnEvent(EventType type, EventContext ctx, int stacks) override {
        if (type != EventType::OnKill) return;
        float hpGain = (float)data["params"]["hpPerKillPerStack"] * stacks;
        ctx.source->m_pStats->bonusHealth += hpGain;
    }
};

// item 20 - DeathMark
// Enemies with 4+ active debuffs take a large damage multiplier for several seconds.
class ItemId_20 : public ItemEffect {
public:
    void OnEvent(EventType type, EventContext ctx, int stacks) override {
        if (type != EventType::OnHit) return;
        if (!ctx.target) return;

        float stackMult = (float)data["params"]["weaknessIncreasePerStack"];
		float multiplier = stackMult * (stacks - 1);

        float baseDur = (float)data["params"]["markDuration"];
        float stackDur = (float)data["params"]["markDurationPerStack"];
        float duration = GetLinearStackingItemValue(baseDur, stackDur, stacks);

        ctx.target->ApplyStatusEffect({ StatusEffectType::DeathMark, duration, nullptr, multiplier });
    }
};


// item 21 - DemonsSoulPrison
// On hit, chance to execute target below an HP threshold.
class ItemId_21 : public ItemEffect {
public:
    void OnEvent(EventType type, EventContext ctx, int stacks) override {
        if (type != EventType::OnHit) return;
        if (!ctx.target) return;

        float threshold = GetLinearStackingItemValue(
			(float)data["params"]["executeThresholdBase"],
            (float)data["params"]["executeThresholdPerStack"],
			stacks);
        float chance = GetLinearStackingItemValue(
			(float)data["params"]["executeChanceBase"],
            (float)data["params"]["executeChancePerStack"],
			stacks);

		//roll
		uniform_real_distribution<float> roll(0, 1);
		if (roll(gen) > chance) return;

        if (ctx.target->GetHealthPercent() <= threshold) {
            ctx.source->DealDamageTo(ctx.target, {
                .damageDealt = -1,
                .healAmount = 0,
                .isCritical = false,
                .isDodged = false,
                .appliesOnHitEffects = false
            });
        }
    }
};

// item 22 - Golden Boots
// On cashout, increase movement speed temporarily
class ItemId_22 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type == EventType::OnCashout) {
			float baseDuration = data["params"]["baseDuration"].get<float>();
			float increasePerStack = data["params"]["increasePerStack"].get<float>();
			float strengthPerStack = data["params"]["strengthIncreasePerStack"].get<float>();
			int speedBoostDuration = ItemEffect::GetLinearStackingItemValue(baseDuration, increasePerStack, stacks);

			StatusEffect effect = StatusEffect();
			effect.duration = speedBoostDuration + 0.5;
			effect.type = StatusEffectType::SpeedBoost;
			effect.strength = strengthPerStack;
			ctx.target->ApplyStatusEffect(effect);
		}
	}
};

// item 23 - Hearty apple
// Increase armor
class ItemId_23 : public ItemEffect {
public:
	void OnPickup(Attackable* owner, int stacks) {
		float baseAmount = data["params"]["baseIncrease"].get<float>();
		owner->m_pStats->armor += baseAmount;
	}

	void OnRemove(Attackable* owner, int stacks) {
		float baseAmount = data["params"]["baseIncrease"].get<float>();
		owner->m_pStats->armor -= baseAmount;
	}
};

// item 24 - Questionable Berries
// Plus attack speed and crit chance, but minus health
class ItemId_24 : public ItemEffect {
public:
	void OnPickup(Attackable* owner, int stacks) {
		float attackSpeedIncrease = data["params"]["attackSpeedIncrease"].get<float>();
		float critIncrease = data["params"]["baseIncrease"].get<float>();
		float healthDecrease = data["params"]["healthDecreasePercent"].get<float>();
		owner->m_pStats->bonusAttackSpeed += attackSpeedIncrease;
		owner->m_pStats->critChance += critIncrease;
		owner->m_pStats->bonusHealth -= healthDecrease;
	}

	void OnRemove(Attackable* owner, int stacks) {
		float attackSpeedIncrease = data["params"]["attackSpeedIncrease"].get<float>();
		float critIncrease = data["params"]["baseIncrease"].get<float>();
		float healthDecrease = data["params"]["healthDecreasePercent"].get<float>();
		owner->m_pStats->bonusAttackSpeed -= attackSpeedIncrease;
		owner->m_pStats->critChance -= critIncrease;
		owner->m_pStats->bonusHealth += healthDecrease;
	}
};

// item 25 - Fire Pendant
// Killing nearby enemy sets you on fire and sharply increases attack speed
class ItemId_25 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnKill) return;
		auto target = context.grid->GetRandomEnemyInRange(ctx.target, 1);
		if (!target) return; //if no close target, no effect

		float effectBase = data["params"]["effectBase"].get<float>();
		float effectIncreasePerStack = data["params"]["effectIncreasePerStack"].get<float>();
		float effectDuration = data["params"]["effectDurationBase"].get<float>();
		float effectDurationPerStack = data["params"]["effectDurationPerStack"].get<float>();
		int duration = ItemEffect::GetLinearStackingItemValue(effectDuration, effectDurationPerStack, stacks);
		int effect = ItemEffect::GetLinearStackingItemValue(effectBase, effectIncreasePerStack, stacks);

		StatusEffect fire = StatusEffect();
		fire.duration = duration - 1;
		fire.type = StatusEffectType::Burning;
		fire.strength = effect;
		ctx.target->ApplyStatusEffect(fire);

		StatusEffect rage = StatusEffect();
		rage.duration = duration;
		rage.type = StatusEffectType::AttackSpeedBoost;
		rage.strength = effect;
		ctx.target->ApplyStatusEffect(rage);
	}
};

// item 26 - Vengant Tongue
// Getting hit below 50% health activates a rampage. Stacks increase duration.
class ItemId_26 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnGettingHit) return;
		if (ctx.target->GetHealthPercent() > 0.5) return; 

		float effectDuration = data["params"]["effectDurationBase"].get<float>();
		float effectDurationPerStack = data["params"]["effectDurationPerStack"].get<float>();
		int duration = ItemEffect::GetLinearStackingItemValue(effectDuration, effectDurationPerStack, stacks);

		StatusEffect rampage = StatusEffect();
		rampage.duration = duration;
		rampage.type = StatusEffectType::Rampage;
		ctx.target->ApplyStatusEffect(rampage);
	}
};

// item 27 - Spore Stone
// Killing an enemy with a status effect affects 2 nearby enemies. Stacks increase strength of the effect that spreads.
class ItemId_27 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnKill) return;
		auto enemyEffects = ctx.target->GetStatusEffects();
		if (enemyEffects.size() == 0) return;

		uniform_int_distribution<int> effectRoll(0, enemyEffects.size() - 1);
		StatusEffect toBeSpread = enemyEffects.at(effectRoll(gen));

		auto target1 = context.grid->GetRandomEnemyInRange(ctx.target, 2);
		auto target2 = context.grid->GetRandomEnemyInRange(ctx.target, 2);

		float durationPercent = data["params"]["durationPercentIncrease"].get<float>();
		float spreadStrength = data["params"]["spreadStrength"].get<float>();
		float spreadStrengthOnStack = data["params"]["spreadStrengthOnStack"].get<float>();
		int strength = ItemEffect::GetLinearStackingItemValue(spreadStrength, spreadStrengthOnStack, stacks);

		StatusEffect effect = StatusEffect();
		effect.duration = toBeSpread.duration * durationPercent;
		effect.type = toBeSpread.type;
		effect.strength = strength;
		target1->ApplyStatusEffect(effect);
		if(target2 != target1) target2->ApplyStatusEffect(effect);
	}
};

// item 28 - Cold Stone
// On hit, chance to effect with freezing
class ItemId_28 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnHit) return;

		float duration = data["params"]["baseDuration"].get<float>();
		float freezeChance = data["params"]["freezeChance"].get<float>();
		float freezeChanceIncreasePerStack = data["params"]["freezeChanceIncreasePerStack"].get<float>();
		int chance = ItemEffect::GetLinearStackingItemValue(freezeChance, freezeChanceIncreasePerStack, stacks);
		uniform_real_distribution<float> freezeRoll(0, 1);

		if (freezeRoll(gen) > chance) return;

		StatusEffect effect = StatusEffect();
		effect.duration = duration;
		effect.type = StatusEffectType::Freezing;
		ctx.target->ApplyStatusEffect(effect);
	}
};

// item 29 - Medkit
// Increase regeneration
class ItemId_29 : public ItemEffect {
public:
	void OnPickup(Attackable* owner, int stacks) {
		float increasePerStack = data["params"]["increasePerStack"].get<float>();
		owner->m_pStats->regernation += increasePerStack;
	}

	void OnRemove(Attackable* owner, int stacks) {
		float increasePerStack = data["params"]["increasePerStack"].get<float>();
		owner->m_pStats->regernation -= increasePerStack;
	}
};

// item 30 - Suspicious mushroom
// Random small chance to be poisoned, but sharply increase damage
class ItemId_30 : public ItemEffect {
public:
	void OnPickup(Attackable* owner, int stacks) {
		auto damageIncreaseMult = data["params"]["damageIncreaseMult"].get<float>();
		auto damageIncreaseMultPerStack = data["params"]["damageIncreaseMultPerStack"].get<float>();
		float amount = stacks > 1 ? damageIncreaseMultPerStack : damageIncreaseMult;
		owner->m_pStats->damageMult += amount;
	}

	void OnRemove(Attackable* owner, int stacks) {
		auto damageIncreaseMult = data["params"]["damageIncreaseMult"].get<float>();
		auto damageIncreaseMultPerStack = data["params"]["damageIncreaseMultPerStack"].get<float>();
		float amount = stacks > 1 ? damageIncreaseMultPerStack : damageIncreaseMult;
		owner->m_pStats->damageMult -= amount;
	}
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnStep) return;

		uniform_real_distribution<float> unluckyRoll(0, 500);
		if (unluckyRoll(gen) != 323) return;

		StatusEffect effect = StatusEffect();
		effect.duration = 10;
		effect.type = StatusEffectType::Poisoning;
		ctx.target->ApplyStatusEffect(effect);
	}
};

// item 31 - Encore
//When below 15% health, become invincible for 1 second. Increase threshold per stack.
class ItemId_31 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnHit) return;
		if (internalTimer > 0) return;

		float thresholdBase = data["params"]["thresholdBase"].get<float>();
		float thresholdIncreasePerStack = data["params"]["thresholdIncreasePerStack"].get<float>();
		float threshold = ItemEffect::GetLinearStackingItemValue(thresholdBase, thresholdIncreasePerStack, stacks);
		if (ctx.target->GetHealthPercent() > threshold) return;

		float invincibleTimePerStack = data["params"]["invincibleTimePerStack"].get<float>();

		float cooldownBase = data["params"]["cooldownBase"].get<float>();
		float cooldownDecreasePerStack = data["params"]["cooldownDecreasePerStack"].get<float>();
		float coolDown = max(cooldownBase - (cooldownDecreasePerStack * (stacks - 1)), 10.0f);

		StatusEffect effect = StatusEffect();
		effect.duration = invincibleTimePerStack * stacks;
		effect.type = StatusEffectType::Invincible;
		ctx.target->ApplyStatusEffect(effect);

		internalTimer = coolDown;
	}
};

// item 32 - Surprise bottle
//50% chance on kill to effect all close by enenemies with poison. Stacks increase poison strength.
class ItemId_32 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnHit) return;
		uniform_int_distribution<int> coinFlip(1, 2);
		if (coinFlip(gen) == 1) return;

		float strengthBase = data["params"]["strengthBase"].get<float>();
		float strengthPerStack = data["params"]["strengthPerStack"].get<float>();
		float strength = ItemEffect::GetLinearStackingItemValue(strengthBase, strengthPerStack, stacks);

		float durationBase = data["params"]["durationBase"].get<float>();
		float durationPerStack = data["params"]["durationPerStack"].get<float>();
		float duration = ItemEffect::GetLinearStackingItemValue(durationBase, durationPerStack, stacks);

		StatusEffect effect = StatusEffect();
		effect.duration = duration;
		effect.type = StatusEffectType::Poisoning;
		effect.strength = strength;

		vector<Collidable*> inRange = context.grid->GetCollidablesInRadius(ctx.target->GetEffectRadiusBound(), ctx.target->GetPosition());
		for (Collidable* c : inRange) {
			if (auto* target = dynamic_cast<Attackable*>(c)) {
				target->ApplyStatusEffect(effect);
			}
		}
	}
};

// item 33 - SpeedGauge
// Every nearby enemy increases your speed temporarily
class ItemId_33 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnStep) return;

		float speedStrengthPerEnemy = data["params"]["speedStrengthPerEnemy"].get<float>();
		float strengthBonus = data["params"]["bonusPerStack"].get<float>();

		int enemyCount = 0;
		vector<Collidable*> nearby = context.grid->GetCollidablesInRadius(ctx.source->GetEffectRadiusBound(), ctx.source->GetPosition());
		for (Collidable* c : nearby) {
			if (dynamic_cast<Enemy*>(c)) enemyCount++;
		}

		if (enemyCount == 0) return;

		float totalStrength = (speedStrengthPerEnemy + strengthBonus) * enemyCount;
		ctx.source->ApplyStatusEffect({ StatusEffectType::SpeedBoost, 1.0f, ctx.source, totalStrength });
	}
};

// item 34 - BirD
// Increases radius for items that effect in a radius
class ItemId_34 : public ItemEffect {
public:
	void OnPickup(Attackable* owner, int stacks) override {
		float increasePerStack = data["params"]["increasePerStack"].get<float>();
		owner->m_pStats->effectRadiusScaler += increasePerStack;
		owner->SetEffectRadiusBound(owner->GetRadius() * owner->m_pStats->effectRadiusScaler);
	}

	void OnRemove(Attackable* owner, int stacks) override {
		float increasePerStack = data["params"]["increasePerStack"].get<float>();
		owner->m_pStats->effectRadiusScaler -= increasePerStack;
		owner->SetEffectRadiusBound(owner->GetRadius() * owner->m_pStats->effectRadiusScaler);
	}
};

// item 35 - SharpEyeCrystal
// On low health, increase crit chance
class ItemId_35 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnStep) return;

		float threshold = GetLinearStackingItemValue(
			data["params"]["healthThresholdBase"].get<float>(),
			data["params"]["healthThresholdIncreasePerStack"].get<float>(),
			stacks);
		float critBonus = data["params"]["critChanceIncrease"].get<float>() * stacks;

		bool belowThreshold = ctx.source->GetHealthPercent() <= threshold;

		if (belowThreshold && !isActive) {
			ctx.source->m_pStats->critChance += critBonus;
			isActive = true;
		}
		else if (!belowThreshold && isActive) {
			ctx.source->m_pStats->critChance -= critBonus;
			isActive = false;
		}
	}
};

// item 36 - ShinyTurtleShell
// On cashout, temporarily increase defense sharply but decrease movement speed
class ItemId_36 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type == EventType::OnCashout) {
			float armorIncrease = data["params"]["armorIncreaseBase"].get<float>();
			float duration = data["params"]["durationPerStack"].get<float>() * stacks;

			ctx.source->m_pStats->armor += armorIncrease;
			ctx.source->ApplyStatusEffect({ StatusEffectType::Slowness, duration, ctx.source, 2.5 });
			internalTimer = duration;
			isActive = true;
		}

		if (type == EventType::OnStep) {
			if (!isActive) return;
			if (internalTimer <= 0) {
				ctx.source->m_pStats->armor -= data["params"]["armorIncreaseBase"].get<float>();
				isActive = false;
			}
		}
	}
};

// item 37 - MoonPie
// On crit, receive rampage effect
class ItemId_37 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnCrit) return;

		float duration = data["params"]["durationPerStack"].get<float>() * stacks;
		ctx.source->ApplyStatusEffect({ StatusEffectType::Rampage, duration, ctx.source });
	}
};

// item 38 - HighPressure
// If poisoned or burning, increase crit chance
class ItemId_38 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnStep) return;

		float critBonus = data["params"]["critChacneIncreasePerStack"].get<float>() * stacks;

		bool hasPoison = false;
		bool hasBurn = false;
		for (auto& effect : ctx.source->GetStatusEffects()) {
			if (effect.type == StatusEffectType::Poisoning) hasPoison = true;
			if (effect.type == StatusEffectType::Burning) hasBurn = true;
		}

		bool shouldBeActive = hasPoison || hasBurn;
		if (shouldBeActive && !isActive) {
			ctx.source->m_pStats->critChance += critBonus;
			isActive = true;
		}
		else if (!shouldBeActive && isActive) {
			ctx.source->m_pStats->critChance -= critBonus;
			isActive = false;
		}
	}
};

// item 39 - SteadyAimTreat
// When frozen, decrease attack speed and sharply increase damage
class ItemId_39 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnStep) return;

		float damageBoost = GetLinearStackingItemValue(
			data["params"]["baseDamageMultIncrease"].get<float>(),
			data["params"]["damageMultIncreasePerStack"].get<float>(),
			stacks);
		float attackSpeedPenalty = -1.0f + (data["params"]["attackSpeedIncreasePerStack"].get<float>() * stacks);

		bool isFrozen = false;
		for (auto& effect : ctx.source->GetStatusEffects()) {
			if (effect.type == StatusEffectType::Freezing) isFrozen = true;
		}

		ctx.source->ApplyStatusEffect({ StatusEffectType::AttackSpeedBoost, 1, ctx.source, attackSpeedPenalty });
		ctx.source->ApplyStatusEffect({ StatusEffectType::DamageBoost, 1, ctx.source, damageBoost });
	}
};

// item 40 - ColdTreat
// On kill, corpse effects nearby enemies in a radius with freezing
class ItemId_40 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnKill) return;

		float duration = GetLinearStackingItemValue(
			data["params"]["freezingDurationBase"].get<float>(),
			data["params"]["freezingDurationPerStack"].get<float>(),
			stacks);

		vector<Collidable*> nearby = context.grid->GetCollidablesInRadius(ctx.target->GetEffectRadiusBound(), ctx.target->GetPosition());

		for (Collidable* c : nearby) {
			if (auto* target = dynamic_cast<Attackable*>(c)) {
				target->ApplyStatusEffect({ StatusEffectType::Freezing, duration, ctx.source });
			}
		}
	}
};

// item 41 - Devil'sFart
// Periodically release a wave of heat affecting nearby enemies with burning
class ItemId_41 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnStep) return;
		if (internalTimer > 0) return;

		float cooldown = data["params"]["cooldownBase"].get<float>() - data["params"]["cooldownDecreasePerStack"].get<float>() * (stacks - 1);
		cooldown = max(cooldown, 5.0f);

		float duration = GetLinearStackingItemValue(
			data["params"]["burningDurationBase"].get<float>(),
			data["params"]["burningDurationPerStack"].get<float>(),
			stacks);
		float strength = data["params"]["burningStrengthPerStack"].get<float>() * stacks;

		vector<Collidable*> nearby = context.grid->GetCollidablesInRadius(
			ctx.source->GetEffectRadiusBound(),
			ctx.source->GetPosition());

		for (Collidable* c : nearby) {
			if (auto* target = dynamic_cast<Enemy*>(c)) {
				target->ApplyStatusEffect({ StatusEffectType::Burning, duration, ctx.source, strength });
			}
		}

		internalTimer = cooldown;
	}
};

// item 42 - GasMask
// Take less damage while poisoned
class ItemId_42 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnGettingHit) return;

		float armorBonus = data["params"]["armorIncreasePerStack"].get<float>() * stacks;

		bool isPoisoned = false;
		for (auto& effect : ctx.source->GetStatusEffects()) {
			if (effect.type == StatusEffectType::Poisoning) isPoisoned = true;
		}

		if (isPoisoned && !isActive) {
			ctx.source->m_pStats->armor += armorBonus;
			isActive = true;
		}
		else if (!isPoisoned && isActive) {
			ctx.source->m_pStats->armor -= armorBonus;
			isActive = false;
		}
	}
};

// item 43 - RuleChanger
// When on fire, also receive rampage effect
class ItemId_43 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnGettingHit) return;

		float strength = data["params"]["rampageStrengthPerStack"].get<float>() * stacks;
		float duration = GetLinearStackingItemValue(
			data["params"]["rampageBonusTimeBase"].get<float>(),
			data["params"]["rampageBonusTimePerStack"].get<float>(),
			stacks);

		bool isBurning = false;
		for (auto& effect : ctx.source->GetStatusEffects()) {
			if (effect.type == StatusEffectType::Burning) isBurning = true;
		}

		if (isBurning && !isActive) {
			ctx.source->ApplyStatusEffect({ StatusEffectType::Rampage, duration, ctx.source, strength });
			isActive = true;
		}
		else if (!isBurning && isActive) {
			isActive = false;
		}
	}
};

// item 44 - PlaybackReel
// Chance on hit to deal double damage. Stacks increase odds.
class ItemId_44 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnHit) return;
		if (!ctx.target) return;

		float chance = data["params"]["chancePerStack"].get<float>() * stacks;
		if (!HasHitChance(chance)) return;

		ctx.source->DealDamageTo(ctx.target, {
			.damageDealt = ctx.hitInfo.damageDealt,
			.healAmount = 0,
			.isCritical = false,
			.isDodged = false,
			.appliesOnHitEffects = true
			});
	}
};

// item 45 - StrengthPotion
// Randomly receive attack boost and attack speed boost
class ItemId_45 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnStep) return;

		float chance = data["params"]["chancePerStack"].get<float>() * stacks;
		if (!HasHitChance(chance)) return;

		float duration = data["params"]["durationBase"].get<float>();
		float strength = data["params"]["strengthPerStack"].get<float>() * stacks;

		ctx.source->ApplyStatusEffect({ StatusEffectType::DamageBoost, duration, ctx.source, strength });
		ctx.source->ApplyStatusEffect({ StatusEffectType::AttackSpeedBoost, duration, ctx.source, strength });
	}
};

// item 46 - LacedDagger
// On hit, deal poison damage
class ItemId_46 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnHit) return;
		if (!ctx.target) return;

		float duration = data["params"]["durationPerStack"].get<float>() * stacks;
		ctx.target->ApplyStatusEffect({ StatusEffectType::Poisoning, duration, ctx.source });
	}
};

// item 47 - DevilsDeal
// Always have rampage effect and death mark. Rampage strength increases with stack.
class ItemId_47 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnStep) return;

		float strength = data["params"]["rampageStrengthPerStack"].get<float>() * stacks;

		ctx.source->ApplyStatusEffect({ StatusEffectType::Rampage, 50.0f, ctx.source, strength });
		ctx.source->ApplyStatusEffect({ StatusEffectType::DeathMark, 50.0f, ctx.source });
	}
};

// item 48 - GodsDeal
// 200% increase to health, 50% decrease to damage. 75% increase to health and 15% decrease to damage per stack.
class ItemId_48 : public ItemEffect {
public:
	void OnPickup(Attackable* owner, int stacks) override {
		float healthIncrease = stacks > 1
			? data["params"]["healthIncreasePerStack"].get<float>()
			: data["params"]["baseHealthIncrease"].get<float>();
		float damageDecrease = stacks > 1
			? data["params"]["damageDecreasePerStack"].get<float>()
			: data["params"]["baseDamageDecrease"].get<float>();

		float healthPercent = owner->GetHealthPercent();
		owner->m_pStats->healthMult += healthIncrease;
		owner->m_pStats->damageMult -= damageDecrease;
		owner->m_pStats->currentHealth = healthPercent * owner->m_pStats->GetFinalHealth();
	}

	void OnRemove(Attackable* owner, int stacks) override {
		float healthIncrease = stacks > 1
			? data["params"]["healthIncreasePerStack"].get<float>()
			: data["params"]["baseHealthIncrease"].get<float>();
		float damageDecrease = stacks > 1
			? data["params"]["damageDecreasePerStack"].get<float>()
			: data["params"]["baseDamageDecrease"].get<float>();

		float healthPercent = owner->GetHealthPercent();
		owner->m_pStats->healthMult -= healthIncrease;
		owner->m_pStats->damageMult += damageDecrease;
		owner->m_pStats->currentHealth = healthPercent * owner->m_pStats->GetFinalHealth();
	}
};

// item 49 - ComboDeal
// Comes with a drink(Standard package boost)
class ItemId_49 : public ItemEffect {
public:
	void OnPickup(Attackable* owner, int stacks) override {
		owner->m_pStats->bonusHealth += data["params"]["healthIncreasePerStack"].get<float>();
		owner->m_pStats->currentHealth += data["params"]["healthIncreasePerStack"].get<float>();
		owner->m_pStats->bonusDamage += data["params"]["damageIncreasePerStack"].get<float>();
		owner->m_pStats->attackSpeedMult += data["params"]["attackSpeedBoostPerStack"].get<float>();
	}

	void OnRemove(Attackable* owner, int stacks) override {
		owner->m_pStats->bonusHealth -= data["params"]["healthIncreasePerStack"].get<float>();
		owner->m_pStats->currentHealth -= data["params"]["healthIncreasePerStack"].get<float>();
		owner->m_pStats->bonusDamage -= data["params"]["damageIncreasePerStack"].get<float>();
		owner->m_pStats->attackSpeedMult -= data["params"]["attackSpeedBoostPerStack"].get<float>();
	}
};

// item 50 - GeishaMask
// Affect enemies with bleeding if attack lands shortly after dodging
class ItemId_50 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type == EventType::OnDodge) {
			float window = GetLinearStackingItemValue(
				data["params"]["attackWindowBase"].get<float>(),
				data["params"]["attackWindowPerStack"].get<float>(),
				stacks);
			internalTimer = window;
			isActive = true;
		}

		if (type == EventType::OnHit) {
			if (!isActive || internalTimer <= 0) return;
			if (!ctx.target) return;

			float bleedDuration = GetLinearStackingItemValue(
				data["params"]["baseBleedDuration"].get<float>(),
				data["params"]["bleedDurationPerStack"].get<float>(),
				stacks);
			ctx.target->ApplyStatusEffect({ StatusEffectType::Bleeding, bleedDuration, ctx.source });
			isActive = false;
		}
	}
};

// item 51 - CowboyHat
// Nearby enemies are slowed
class ItemId_51 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnStep) return;

		float strength = GetLinearStackingItemValue(
			data["params"]["baseSlowStrength"].get<float>(),
			data["params"]["slowStrengthPerStack"].get<float>(),
			stacks);

		vector<Collidable*> nearby = context.grid->GetCollidablesInRadius(
			ctx.source->GetEffectRadiusBound(),
			ctx.source->GetPosition());

		for (Collidable* c : nearby) {
			if (auto* target = dynamic_cast<Enemy*>(c)) {
				target->ApplyStatusEffect({ StatusEffectType::Slowness, 1.0f, ctx.source, strength });
			}
		}
	}
};

// item 52 - PestilentHelm
// Nearby enemies take more damage
class ItemId_52 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnStep) return;

		float weakness = data["params"]["weaknessPerStack"].get<float>() * stacks;

		vector<Collidable*> nearby = context.grid->GetCollidablesInRadius(
			ctx.source->GetEffectRadiusBound(),
			ctx.source->GetPosition());

		for (Collidable* c : nearby) {
			if (auto* target = dynamic_cast<Enemy*>(c)) {
				target->ApplyStatusEffect({ StatusEffectType::DeathMark, 1.0f, ctx.source, weakness });
			}
		}
	}
};

// item 53 - Pepper
// After not taking damage for a period of time, receive movement speed boost
class ItemId_53 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type == EventType::OnGettingHit) {
			internalTimer = data["params"]["timeAfterLastDamage"].get<float>();
			if (isActive) {
				float strength = data["params"]["speedBoostStengthPerStack"].get<float>() * stacks;
				ctx.source->m_pStats->speedMult /= 1.0f + strength;
				isActive = false;
			}
		}

		if (type == EventType::OnStep) {
			if (isActive || internalTimer > 0) return;
			float strength = data["params"]["speedBoostStengthPerStack"].get<float>() * stacks;
			ctx.source->m_pStats->speedMult *= 1.0f + strength;
			isActive = true;
		}
	}
};

// item 54 - Adrenaline
// On getting hit, slightly increase movement speed permanently
class ItemId_54 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnGettingHit) return;
		float speedIncrease = data["params"]["speedIncreasePerStack"].get<float>() * stacks;
		ctx.source->m_pStats->bonusSpeed += speedIncrease;
	}
};


// item 55 - OvercalculatedShot
// Sharply increase crit damage, but decrease armor
class ItemId_55 : public ItemEffect {
public:
	void OnPickup(Attackable* owner, int stacks) override {
		owner->m_pStats->critMultiplyer += data["params"]["critMultIncreasePerStack"].get<float>();
		owner->m_pStats->armor -= data["params"]["armorDecreasePerStack"].get<float>();
	}

	void OnRemove(Attackable* owner, int stacks) override {
		owner->m_pStats->critMultiplyer -= data["params"]["critMultIncreasePerStack"].get<float>();
		owner->m_pStats->armor += data["params"]["armorDecreasePerStack"].get<float>();
	}
};

// item 56 - Crusader
// Increase damage based on the difference of armor and weakness
class ItemId_56 : public ItemEffect {
public:
	void OnModifyStats(StatSheet& stats, int stacks) override {
		float diff = max(0.0f, (float)(stats.armor - stats.weakness));
		stats.bonusDamage += diff * data["params"]["damageIncreasePerDifference"].get<float>() * stacks;
	}
};

// item 57 - Symbiote
// On heal, 5% chance to be poisoned and increase damage permanently
class ItemId_57 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnHeal) return;
		if (!HasHitChance(0.05f)) return;

		float duration = GetLinearStackingItemValue(
			data["params"]["basePoisonDuration"].get<float>(),
			data["params"]["poisonDurationPerStack"].get<float>(),
			stacks);

		ctx.source->m_pStats->damageMult += data["params"]["damageMultIncreasePerStack"].get<float>();
		ctx.source->ApplyStatusEffect({ StatusEffectType::Poisoning, duration, ctx.source });
	}
};

// item 58 - RetaliationNecklace
// Dodging shortly after being hit temporarily boosts damage
class ItemId_58 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type == EventType::OnGettingHit) {
			float window = GetLinearStackingItemValue(
				data["params"]["baseDodgeWindowTime"].get<float>(),
				data["params"]["dodgeWindowTimePerStack"].get<float>(),
				stacks);
			internalTimer = window;
			isActive = true;
		}

		if (type == EventType::OnDodge) {
			if (!isActive || internalTimer <= 0) return;

			float duration = data["params"]["durationPerStack"].get<float>() * stacks;
			float boost = GetLinearStackingItemValue(
				data["params"]["baseDamageBoost"].get<float>(),
				data["params"]["damageBoostPerStack"].get<float>(),
				stacks);

			ctx.source->ApplyStatusEffect({ StatusEffectType::DamageBoost, duration, ctx.source, boost });
			isActive = false;
		}
	}
};

// item 59 - AthleticCloak
// Small chance on hit to receive a large attack speed boost, but all nearby enemies also receive a speed boost
class ItemId_59 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnHit) return;

		float chance = data["params"]["chancePerStack"].get<float>() * stacks;
		if (!HasHitChance(chance)) return;

		float duration = GetLinearStackingItemValue(
			data["params"]["baseDuration"].get<float>(),
			data["params"]["durationPerStack"].get<float>(),
			stacks);
		float attackBoost = GetLinearStackingItemValue(
			data["params"]["baseAttackSpeedBoost"].get<float>(),
			data["params"]["attackSpeedBoostPerStack"].get<float>(),
			stacks);

		ctx.source->ApplyStatusEffect({ StatusEffectType::AttackSpeedBoost, duration, ctx.source, attackBoost });

		vector<Collidable*> nearby = context.grid->GetCollidablesInRadius(
			ctx.source->GetEffectRadiusBound(),
			ctx.source->GetPosition());

		for (Collidable* c : nearby) {
			if (auto* target = dynamic_cast<Enemy*>(c)) {
				target->ApplyStatusEffect({ StatusEffectType::SpeedBoost, duration, ctx.source, data["params"]["enemySpeedStrength"].get<float>() });
			}
		}
	}
};

// item 60 - HeavyGreives
// Triple crit chance, but dodging deals 25% of your health. Stacks decrease damage taken from dodging.
class ItemId_60 : public ItemEffect {
public:
	void OnPickup(Attackable* owner, int stacks) override {
		if (stacks == 1) owner->m_pStats->critChance *= 3.0f;
	}

	void OnRemove(Attackable* owner, int stacks) override {
		if (stacks == 0) owner->m_pStats->critChance /= 3.0f;
	}

	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnDodge) return;

		float damagePercent = max(0.0f,
			data["params"]["baseDamagePercent"].get<float>() -
			data["params"]["damagerPercentDecreasePerStack"].get<float>() * (stacks - 1));

		float damage = ctx.source->m_pStats->GetFinalHealth() * damagePercent;
		ctx.source->DealDamageTo(ctx.source, {
			.damageDealt = damage,
			.healAmount = 0,
			.isCritical = false,
			.isDodged = false,
			.appliesOnHitEffects = false
			});
	}
};

// item 61 - BrittleCrown
// Increase all stats, but chance on hit to receive any random effect
class ItemId_61 : public ItemEffect {
public:
	void OnPickup(Attackable* owner, int stacks) override {
		float increase = data["params"]["percentIncerasePerStack"].get<float>();
		owner->m_pStats->healthMult += increase;
		owner->m_pStats->damageMult += increase;
		owner->m_pStats->speedMult += increase;
		owner->m_pStats->attackSpeedMult += increase;
	}

	void OnRemove(Attackable* owner, int stacks) override {
		float increase = data["params"]["percentIncerasePerStack"].get<float>();
		owner->m_pStats->healthMult -= increase;
		owner->m_pStats->damageMult -= increase;
		owner->m_pStats->speedMult -= increase;
		owner->m_pStats->attackSpeedMult -= increase;
	}

	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnGettingHit) return;

		float chance = data["params"]["onHitChancePerStack"].get<float>() * stacks;
		if (!HasHitChance(chance)) return;

		static const vector<StatusEffectType> allEffects = {
			StatusEffectType::Bleeding, StatusEffectType::Burning,
			StatusEffectType::Poisoning, StatusEffectType::Freezing,
			StatusEffectType::Slowness, StatusEffectType::SpeedBoost,
			StatusEffectType::DamageBoost, StatusEffectType::AttackSpeedBoost,
			StatusEffectType::Rampage, StatusEffectType::Invincible
		};

		uniform_int_distribution<int> roll(0, allEffects.size() - 1);
		StatusEffectType randomEffect = allEffects[roll(gen)];
		ctx.source->ApplyStatusEffect({ randomEffect, 5.0f, ctx.source, 1.0f });
	}
};

// item 62 - FocusFicus
// Crit chance increases after standing still for 1 second
class ItemId_62 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnStep) return;

		bool isStill = ctx.source->GetVelocity().x == 0 && ctx.source->GetVelocity().y == 0;

		if (!isStill) {
			internalTimer = 1.0f; // reset countdown
			if (isActive) {
				float critBonus = GetLinearStackingItemValue(
					data["params"]["baseIncrease"].get<float>(),
					data["params"]["increasePerStack"].get<float>(),
					stacks);
				ctx.source->m_pStats->critChance -= critBonus;
				isActive = false;
			}
		}
		else if (internalTimer <= 0 && !isActive) {
			float critBonus = GetLinearStackingItemValue(
				data["params"]["baseIncrease"].get<float>(),
				data["params"]["increasePerStack"].get<float>(),
				stacks);
			ctx.source->m_pStats->critChance += critBonus;
			isActive = true;
		}
	}
};

// item 63 - Fih
// When freezing effect ends, receive poison effect and convert to spoiled fih
class ItemId_63 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnStep) return;

		bool isFrozen = false;
		for (auto& effect : ctx.source->GetStatusEffects()) {
			if (effect.type == StatusEffectType::Freezing) isFrozen = true;
		}

		if (isFrozen) {
			isActive = true; // was frozen at some point
		}

		if (!isFrozen && isActive) {
			float duration = data["params"]["poisonDuration"].get<float>();
			ctx.source->ApplyStatusEffect({ StatusEffectType::Poisoning, duration, ctx.source });
			ctx.source->AddItem(64, 1);
			ctx.source->RemoveItem(63, 1);
			isActive = false;
		}
	}
};

// item 64 - SpoiledFih
// Below 25% health, consume fih and fully heal. Receive temporary bleeding and sharp permanent health boost.
class ItemId_64 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnStep) return;
		if (ctx.source->GetHealthPercent() > 0.25f) return;

		float bleedDuration = data["params"]["bleedingDuration"].get<float>();
		float healthBoost = data["params"]["healthMultIncrease"].get<float>();

		EventContext healCtx = {
			.source = ctx.source,
			.target = ctx.source,
			.hitInfo = { 0, -1, false, false }
		};
		ctx.source->ApplyHeal(healCtx);
		ctx.source->ApplyStatusEffect({ StatusEffectType::Bleeding, bleedDuration, ctx.source });
		ctx.source->m_pStats->healthMult += healthBoost;

		ctx.source->RemoveItem(63, 1);
	}
};

// item 65 - ChefHat
// On enemy kill, chance to cook a consumable food item
class ItemId_65 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnKill) return;

		float chance = GetLinearStackingItemValue(
			data["params"]["baseChance"].get<float>(),
			data["params"]["chancePerStack"].get<float>(),
			stacks);
		if (!HasHitChance(chance)) return;

		static const vector<int> foodItems = { 66, 67, 68, 69, 70, 71, 72, 73 };
		uniform_int_distribution<int> roll(0, foodItems.size() - 1);
		int chosenFood = foodItems[roll(gen)];

		auto itemDef = context.ir->Get(chosenFood);
		auto newItem = new Item(itemDef.data);
		newItem->SetBasePos(ctx.source->GetPosition());
		context.grid->UpdateOccupancy((Entity*)newItem, &GridCell::AddOther, &GridCell::RemoveOther);
	}
};

// item 66 - Milk
// On heal, consume and add 10 armor
class ItemId_66 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnHeal) return;

		ctx.source->m_pStats->armor += 10;
		ctx.source->RemoveItem(66, 1);
	}
};

// item 67 - Pineapple
// When incoming attack deals more than 25% of health, consume and deal 50% health to source
class ItemId_67 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnGettingHit) return;

		float maxHealth = ctx.target->m_pStats->GetFinalHealth();
		if (ctx.hitInfo.damageDealt < maxHealth * 0.25f) return;

		float retaliation = maxHealth * 0.5f;
		ctx.target->DealDamageTo(ctx.source, {
			.damageDealt = retaliation,
			.healAmount = 0,
			.isCritical = false,
			.isDodged = false,
			.appliesOnHitEffects = false
			});
		ctx.target->RemoveItem(67, 1);
	}
};

// item 68 - SpicyPepper
// When below 20% health, consume and receive speed boost
class ItemId_68 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnStep) return;
		if (ctx.source->GetHealthPercent() > 0.2f) return;

		float strength = data["params"]["speedBoostStrength"].get<float>();
		float duration = data["params"]["speedBoostDuration"].get<float>();
		ctx.source->ApplyStatusEffect({ StatusEffectType::SpeedBoost, duration, ctx.source, strength });
		ctx.source->RemoveItem(68, 1);
	}
};

// item 69 - Sushi
// When dodging below 50% health, consume and receive 3 seconds of invincibility
class ItemId_69 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnDodge) return;
		if (ctx.source->GetHealthPercent() > 0.5f) return;

		ctx.source->ApplyStatusEffect({ StatusEffectType::Invincible, 3.0f, ctx.source });
		ctx.source->RemoveItem(69, 1);
	}
};

// item 70 - Onion
// When poisoned, consume and cure poison
class ItemId_70 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnStep) return;

		auto effects = ctx.source->GetStatusEffects();
		bool isPoisoned = false;
		for (auto& effect : effects) {
			if (effect.type == StatusEffectType::Poisoning) isPoisoned = true;
		}
		if (!isPoisoned) return;

		std::erase_if(effects, [](const StatusEffect& s) {
			return s.type == StatusEffectType::Poisoning;
			});
		ctx.source->RemoveItem(70, 1);
	}
};

// item 71 - HeartySoup
// When frozen or burning, consume and cure yourself
class ItemId_71 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnStep) return;

		auto effects = ctx.source->GetStatusEffects();
		bool isFrozen = false;
		bool isBurning = false;
		for (auto& effect : effects) {
			if (effect.type == StatusEffectType::Freezing) isFrozen = true;
			if (effect.type == StatusEffectType::Burning) isBurning = true;
		}
		if (!isFrozen && !isBurning) return;

		std::erase_if(effects, [](const StatusEffect& s) {
			return s.type == StatusEffectType::Freezing || s.type == StatusEffectType::Burning;
			});
		ctx.source->RemoveItem(71, 1);
	}
};

// item 72 - Melon
// If its Thursday, on hit eat and heal fully. Otherwise, give 3% regeneration boost per stack.
// NOTE: This is such a stupid fucking item, fuck this project lmao
class ItemId_72 : public ItemEffect {
public:
	void OnPickup(Attackable* owner, int stacks) override {
		if (!IsThursday()) {
			owner->m_pStats->regernation += 0.03f;
		}
	}

	void OnRemove(Attackable* owner, int stacks) override {
		if (!IsThursday()) {
			owner->m_pStats->regernation -= 0.03f;
		}
	}

	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnHit) return;
		if (!IsThursday()) return;

		EventContext healCtx = {
			.source = ctx.source,
			.target = ctx.source,
			.hitInfo = { 0, -1, false, false }
		};
		ctx.source->ApplyHeal(healCtx);
		ctx.source->RemoveItem(72, 1);
	}

private:
	bool IsThursday() {
		std::time_t t = std::time(nullptr);
        #pragma warning(suppress: 4996)
		std::tm* now = std::localtime(&t);
		return now->tm_wday == 4; // 0 = Sunday, 4 = Thursday
	}
};

// item 73 - Bread
// On enemy kill, 10% chance to get hungry and eat, granting a short attack boost
class ItemId_73 : public ItemEffect {
public:
	void OnEvent(EventType type, EventContext ctx, int stacks) override {
		if (type != EventType::OnKill) return;
		if (!HasHitChance(0.1f)) return;

		float duration = data["params"]["attackBoostDuration"].get<float>();
		float strength = data["params"]["attackBoostStrength"].get<float>();
		ctx.source->ApplyStatusEffect({ StatusEffectType::AttackSpeedBoost, duration, ctx.source, strength });
		ctx.source->RemoveItem(73, 1);
	}
};

static void GiveRandomFoodItems(Attackable* owner, int count) {
	static const vector<int> foodItems = { 66, 67, 68, 69, 70, 71, 72, 73 };
	uniform_int_distribution<int> roll(0, foodItems.size() - 1);
	for (int i = 0; i < count; i++) {
		owner->AddItem(foodItems[roll(gen)], 1);
	}
}

// item 74 - EmptyPlate
// Grants 3 random consumable food items
class ItemId_74 : public ItemEffect {
public:
	void OnPickup(Attackable* owner, int stacks) override {
		GiveRandomFoodItems(owner, 3);
		owner->RemoveItem(74, 1);
	}
};

// item 75 - EmptyPan
// Grants 5 random consumable food items
class ItemId_75 : public ItemEffect {
public:
	void OnPickup(Attackable* owner, int stacks) override {
		GiveRandomFoodItems(owner, 5);
		owner->RemoveItem(75, 1);
	}
};

// item 76 - EmptyCauldron
// Grants 10 random consumable food items
class ItemId_76 : public ItemEffect {
public:
	void OnPickup(Attackable* owner, int stacks) override {
		GiveRandomFoodItems(owner, 10);
		owner->RemoveItem(76, 1);
	}
};