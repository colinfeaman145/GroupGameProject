#include "BisonSteak.hpp"

#include "ItemEffect.hpp"
#include "Attackable.hpp"
#include "StatSheet.hpp"


void BisonSteak::OnPickup(Attackable* owner, int stacks) {
}

void BisonSteak::OnRemove(Attackable* owner, int stacks) {
}

// adds base health per stack
void BisonSteak::OnModifyStats(StatSheet& stats, int stacks) {
	auto baseHealth = data["params"]["baseHealth"].get<float>();
	auto increasePerStack = data["params"]["increasePerStack"].get<float>();

	stats.bonusHealth = ItemEffect::GetLinearStackingItemValue(baseHealth, increasePerStack, stacks);
}

void BisonSteak::OnEvent(EventType type, EventContext ctx, int stacks) {

}
