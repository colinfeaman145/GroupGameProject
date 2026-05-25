#include "ItemSpawner.hpp"
#include "GameContext.hpp"
#include "ItemRegistry.hpp"
#include "Item.hpp"


void ItemSpawner::OnPickup(Attackable* owner, int stacks)
{
}

void ItemSpawner::OnRemove(Attackable* owner, int stacks)
{
}

void ItemSpawner::OnModifyStats(StatSheet& stats, int stacks)
{
}

void ItemSpawner::OnEvent(EventType type, EventContext ctx, int stacks) {
	if (type == EventType::OnAttack) {
		// spawn item on kill

		auto id = data["params"]["itemToSpawn"];
		auto itemDef = context.ir->Get(id);
		auto newItem = new Item(itemDef.data);
		newItem->SetBasePos(ctx.targetPosition);
		context.currentScene->AddElement(newItem);
	}
}
