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

//// item 22 - Golden Boots
//// On cashout, increase movement speed temporarily
//class ItemId_22 : public ItemEffect {
//public:
//	void OnEvent(EventType type, EventContext ctx, int stacks) override {
//		if (type == EventType::OnCashout) {
//			auto baseDuration = data["params"]["baseDuration"].get<float>();
//			auto increasePerStack = data["params"]["increasePerStack"].get<float>();
//			int speedBoostDuration = ItemEffect::GetLinearStackingItemValue(baseDuration, increasePerStack, stacks);
//
//			StatusEffect effect = StatusEffect();
//			effect.duration = speedBoostDuration;
//			effect.type = StatusEffectType::SpeedBoost;
//			ctx.target->ApplyStatusEffect(effect);
//		}
//	}
//};
