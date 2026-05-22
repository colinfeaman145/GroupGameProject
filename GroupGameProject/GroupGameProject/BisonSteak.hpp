
#pragma once
#include "ItemEffect.hpp"
#include "Attackable.hpp"

class StatSheet;

class BisonSteak : public ItemEffect {
	void OnPickup(Attackable* owner, int stacks) {
	}

	void OnRemove(Attackable* owner, int stacks) {
	}

	void OnModifyStats(StatSheet& stats, int stacks) {
		stats.bonusHealth = ItemEffect::GetLinearStackingItemValue(50, 20, stacks);
	}

	void OnEvent(EventType type, EventContext ctx, int stacks) {

	}
};
