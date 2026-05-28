#include "InvincibleModule.hpp"
#include "Attackable.hpp"

void InvincibleModule::OnPickup(Attackable* owner, int stacks) {
}

void InvincibleModule::OnRemove(Attackable* owner, int stacks) {
}

void InvincibleModule::OnModifyStats(StatSheet& stats, int stacks) {
}

// makes the owner invincible for a short amout of time (configurable in json file)
// prevents the system to apply damage to the player every frame
void InvincibleModule::OnEvent(EventType type, EventContext ctx, int stacks) {
	if (type != EventType::OnGettigHit) return;
	ctx.target->ApplyStatusEffect(StatusEffectType::Invincible, data["params"]["duration"], ctx.target);

}
