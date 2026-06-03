#pragma once
#include <math.h>
#include "Vector2.hpp"

#include "json.hpp"
using json = nlohmann::json;

class Attackable;
class StatSheet;

enum StatusEffectType {
	DeathMark,
	DamageBoost,
	AttackSpeedBoost,
	Invincible,
	Bleeding,
	Burning,
	Poisoning,
	Freezing,
	Shocking,
	SpeedBoost,
	Slowness,
	Rampage
};
struct StatusEffect {
	StatusEffectType type;
	float duration;
	Attackable* source;
	bool hasStarted = false;
};

enum EventType {
	OnCashout,
	OnHit,
	OnGettingHit,
	OnKill,
	OnCrit,
	OnAttack,
	OnDeath,
	OnHeal,
	OnStep,
	OnDodge
};

struct HitInfo {
	float damageDealt;
	float healAmount;
	bool isCritical;
	bool isDodged;
	bool appliesOnHitEffects;
};

struct EventContext {
	Attackable* source;
	Attackable* target;
	Vector2 targetPosition;
	HitInfo hitInfo;
};

class ItemEffect {
public:
	virtual void OnPickup(Attackable* owner, int stacks) {}
	virtual void OnRemove(Attackable* owner, int stacks) {}
	virtual void OnModifyStats(StatSheet& stats, int stacks) {}
	virtual void OnEvent(EventType type, EventContext ctx, int stacks) {}

	// used to register item effects from json data. T must be derived from ItemEffect and have a default constructor
	template<typename T>
	static T* CreateItemEffectFromJson(json data) {
		auto newItem = new T();
		if (auto item = dynamic_cast<ItemEffect*>(newItem)) {
			item->data = data;
		}
		else {
			delete newItem;
			throw std::runtime_error("Type T must be derived from ItemEffect");
		}
		return newItem;
	}

	static float GetLinearStackingItemValue(float itemBase, int perStack, int stacks) {
		return itemBase + perStack * (stacks - 1);
	}
	static float GetHyperbolicStackingItemValue(float perStack, int stacks) {
		return 1 - pow(1 - perStack, stacks);
	}

protected:
	json data;
};




