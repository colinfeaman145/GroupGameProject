
#pragma once
#include "ItemEffect.hpp"
#include "Attackable.hpp"
#include "json.hpp"

using json = nlohmann::json;

class StatSheet;

class BisonSteak : public ItemEffect {
	void OnPickup(Attackable* owner, int stacks) {
	}

	void OnRemove(Attackable* owner, int stacks) {
	}

	void OnModifyStats(StatSheet& stats, int stacks) {
		auto baseHealth = data["params"]["baseHealth"].get<float>();
		auto increasePerStack = data["params"]["increasePerStack"].get<float>();

		stats.bonusHealth = ItemEffect::GetLinearStackingItemValue(baseHealth, increasePerStack, stacks);
	}

	void OnEvent(EventType type, EventContext ctx, int stacks) {

	}
};
