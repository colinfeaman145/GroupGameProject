#include "ItemId_2.hpp"

#include "ItemEffect.hpp"
#include "Attackable.hpp"
#include "StatSheet.hpp"


void ItemId_2::OnPickup(Attackable* owner, int stacks) {
	auto baseHealth = data["params"]["baseHealth"].get<float>();
	auto increasePerStack = data["params"]["increasePerStack"].get<float>();
	owner->m_pStats->currentHealth += stacks > 1 ? increasePerStack : baseHealth;
}

void ItemId_2::OnRemove(Attackable* owner, int stacks) {
	auto baseHealth = data["params"]["baseHealth"].get<float>();
	auto increasePerStack = data["params"]["increasePerStack"].get<float>();
	owner->m_pStats->currentHealth -= stacks > 1 ? increasePerStack : baseHealth;
}

// adds base health per stack
void ItemId_2::OnModifyStats(StatSheet& stats, int stacks) {
	auto baseHealth = data["params"]["baseHealth"].get<float>();
	auto increasePerStack = data["params"]["increasePerStack"].get<float>();

	stats.bonusHealth += ItemEffect::GetLinearStackingItemValue(baseHealth, increasePerStack, stacks);
}

void ItemId_2::OnEvent(EventType type, EventContext ctx, int stacks) {

}
