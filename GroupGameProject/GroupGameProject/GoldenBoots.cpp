#include "GoldenBoots.hpp"
#include "Attackable.hpp"
#include "StatSheet.hpp"

void GoldenBoots::OnPickup(Attackable* owner, int stacks) {
}

void GoldenBoots::OnRemove(Attackable* owner, int stacks) {
}

void GoldenBoots::OnModifyStats(StatSheet& stats, int stacks) {
}

void GoldenBoots::OnEvent(EventType type, EventContext ctx, int stacks) {
	if (type == EventType::OnCashout) {
		auto baseDuration = data["params"]["baseDuration"].get<float>();
		auto increasePerStack = data["params"]["increasePerStack"].get<float>();
		int speedBoostDuration = ItemEffect::GetLinearStackingItemValue(baseDuration, increasePerStack, stacks);

		ctx.target->ApplyStatusEffect(StatusEffectType::SpeedBoost, speedBoostDuration, ctx.target);
	}
}
