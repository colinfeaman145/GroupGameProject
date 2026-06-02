#include "ItemId_7.hpp"
#include "GameContext.hpp"
#include "Attackable.hpp"
#include "StatSheet.hpp"

void ItemId_7::OnPickup(Attackable* owner, int stacks) {
	// update current health
	float healthIncrease = data["params"]["healthIncreasePerLevel"];
	owner->m_pStats->currentHealth += (owner->m_pStats->defaultBaseHealth * healthIncrease);
}

void ItemId_7::OnRemove(Attackable* owner, int stacks) {
	float healthIncrease = data["params"]["healthIncreasePerLevel"];
	owner->m_pStats->currentHealth -= (owner->m_pStats->defaultBaseHealth * healthIncrease);
}

void ItemId_7::OnModifyStats(StatSheet& stats, int stacks) {
	float healthIncrease = data["params"]["healthIncreasePerLevel"];
	float damageIncrease = data["params"]["damageIncreasePerLevel"];
	stats.baseHealth = stats.defaultBaseHealth + (stats.defaultBaseHealth * healthIncrease) * stacks;
	stats.baseDamage = stats.defaultBaseDamage + (stats.defaultBaseDamage * damageIncrease) * stacks;

}

void ItemId_7::OnEvent(EventType type, EventContext ctx, int stacks) {}
