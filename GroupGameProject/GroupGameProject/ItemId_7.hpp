#pragma once
#include "ItemEffect.hpp"

class ItemId_7 : public ItemEffect {
	virtual void OnPickup(Attackable* owner, int stacks) override;
	virtual void OnRemove(Attackable* owner, int stacks) override;
	virtual void OnModifyStats(StatSheet& stats, int stacks) override;
	virtual void OnEvent(EventType type, EventContext ctx, int stacks) override;
};

