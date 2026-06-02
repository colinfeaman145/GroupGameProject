#pragma once

#include "ItemEffect.hpp"

class ItemId_8 : public ItemEffect {
	void OnPickup(Attackable* owner, int stacks) {}
	void OnRemove(Attackable* owner, int stacks) {}
	void OnModifyStats(StatSheet& stats, int stacks) {}
	void OnEvent(EventType type, EventContext ctx, int stacks) {}
};