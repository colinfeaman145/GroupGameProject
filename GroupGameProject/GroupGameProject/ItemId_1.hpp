#pragma once
#include "ItemEffect.hpp"

class ItemId_1 : public ItemEffect {
	void OnPickup(Attackable* owner, int stacks) override;
	void OnRemove(Attackable* owner, int stacks) override;
	void OnModifyStats(StatSheet& stats, int stacks) override;
	void OnEvent(EventType type, EventContext ctx, int stacks) override;
};

