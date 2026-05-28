#include "ItemId_6.hpp"
#include "StatSheet.hpp"

void ItemId_6::OnPickup(Attackable* owner, int stacks)
{
}

void ItemId_6::OnRemove(Attackable* owner, int stacks)
{
}

void ItemId_6::OnModifyStats(StatSheet& stats, int stacks) {
	auto increasePerStack = data["params"]["increasePerStack"].get<float>();
	stats.attackSpeedMult += ItemEffect::GetHyperbolicStackingItemValue(increasePerStack, stacks);
}

void ItemId_6::OnEvent(EventType type, EventContext ctx, int stacks)
{
}
