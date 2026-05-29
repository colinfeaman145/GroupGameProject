#include "ItemId_5.hpp"
#include "Attackable.hpp"

void ItemId_5::OnPickup(Attackable* owner, int stacks) {
}

void ItemId_5::OnRemove(Attackable* owner, int stacks) {
}

void ItemId_5::OnModifyStats(StatSheet& stats, int stacks) {
}

// makes the owner invincible for a short amout of time (configurable in json file)
// prevents the system to apply damage to the player every frame
void ItemId_5::OnEvent(EventType type, EventContext ctx, int stacks) {
	if (type != EventType::OnGettigHit) return;
	ctx.target->ApplyStatusEffect(StatusEffectType::Invincible, data["params"]["duration"], ctx.target);

}
