#pragma once
#include "StatSheet.hpp"
#include <math.h>
#include "Vector2.hpp"

class Attackable;

enum StatusEffectType {
	Bleeding,
	Burning,
	Poisoning,
	Freezing,
	Shocking
};
struct StatusEffect {
	StatusEffectType type;
	float duration;
	Attackable* source;
};

enum EventType {
	OnHit,
	OnGettigHit,
	OnKill,
	OnCrit,
	OnHeal,
	OnStep,
	OnAttack,
	OnDeath,
	OnDodge
};

struct HitInfo {
	float damageDealt;
	bool isCritical;
	bool isDodged;
};

struct EventContext {
	Attackable* source;
	Attackable* target;
	Vector2 targetPosition;
	HitInfo hitInfo;
};

class ItemEffect {
public:
	virtual void OnPickup(Attackable* owner, int stacks) = 0;
	virtual void OnRemove(Attackable* owner, int stacks) = 0;
	virtual void OnModifyStats(StatSheet& stats, int stacks) = 0;
	virtual void OnEvent(EventType type, EventContext ctx, int stacks) = 0;

	static float GetLinearStackingItemValue(float itemBase, int perStack, int stacks) {
		return itemBase + perStack * (stacks - 1);
	}
	static float GeHyperbolicStackingItemValue(int perStack, int stacks) {
		return 1 - pow(1 - perStack, stacks);
	}
};




