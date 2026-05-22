#pragma once
#include "ItemEffect.hpp"
#include "Attackable.hpp"

class StatSheet;

class Gun : public ItemEffect {
	void OnPickup(Attackable* owner, int stacks) {
	}

	void OnRemove(Attackable* owner, int stacks) {
	}

	void OnModifyStats(StatSheet& stats, int stacks) {

	}

	void OnEvent(EventType type, EventContext ctx, int stacks) {
		if (type != EventType::OnAttack) {
			return;
		}

		ctx.target->ApplyDamage({ 10.f * stacks , false, false });
	}
};

