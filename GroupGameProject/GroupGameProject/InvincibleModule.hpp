#pragma once
#include "ItemEffect.hpp"

class InvincibleModule : public ItemEffect {
	virtual void OnPickup(Attackable* owner, int stacks) override;
	virtual void OnRemove(Attackable* owner, int stacks) override;
	virtual void OnModifyStats(StatSheet& stats, int stacks) override;
	virtual void OnEvent(EventType type, EventContext ctx, int stacks) override;
};

