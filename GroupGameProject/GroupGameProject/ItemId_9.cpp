#include "ItemId_9.hpp"
#include "Attackable.hpp"

void ItemId_9::OnPickup(Attackable* owner, int stacks) {
}

void ItemId_9::OnRemove(Attackable* owner, int stacks) {
}

void ItemId_9::OnModifyStats(StatSheet& stats, int stacks) {
}

void ItemId_9::OnEvent(EventType type, EventContext ctx, int stacks) {
    if (type != EventType::OnGettingHit) return;
    float reflectPercent = GetHyperbolicStackingItemValue(data["reflectPercentage"], stacks); 
    float reflectDamage = ctx.hitInfo.damageDealt * reflectPercent;

	HitInfo hitInfo = {
		.damageDealt = reflectDamage,
		.healAmount = 0,
		.isCritical = false,
		.isDodged = false
	};
    if (ctx.source) ctx.source->DealDamageTo(ctx.target, hitInfo);
}
