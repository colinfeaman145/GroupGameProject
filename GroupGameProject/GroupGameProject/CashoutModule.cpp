#include "CashoutModule.hpp"
#include "Attackable.hpp";
#include "GameContext.hpp"

void CashoutModule::OnPickup(Attackable* owner, int stacks) {
}

void CashoutModule::OnRemove(Attackable* owner, int stacks) {
}

void CashoutModule::OnModifyStats(StatSheet& stats, int stacks) {
}

void CashoutModule::OnEvent(EventType type, EventContext ctx, int stacks) {
	if (type == EventType::OnKill) {
		ctx.source->AddItem(data["id"], 1);
	}

	if (type == EventType::OnCashout) {
		auto count = ctx.source->GetItemCount(data["id"]);

		// find current tier
		auto currentTier = 0;
		json currentTierConfig;
		auto currentKillCount = ctx.source->GetItemCount(data["id"]) - 1;
		for (auto item : data["params"]["cashoutTiers"]) {
			if (currentKillCount < item["threshold"]) break;
			currentTierConfig = item;
			currentTier++;
		}
		if (currentTier == 0) return;
		 
		// generate items for the player depending on item tier chance of current tier
		std::vector<ItemDef> itemsToSpawn;
		std::uniform_real_distribution<float> dist(0.0f, 1.0f);
		auto itemCountForPlayer = currentTierConfig["randomItemCount"].get<int>() + currentTierConfig["selectableItemCount"].get<int>();
		for (int i = 0; i < itemCountForPlayer; i++) {
			auto chanceHit = dist(gen);
			if (chanceHit <= currentTierConfig["legendaryChance"]) {
				itemsToSpawn.push_back(context.ir->GetRandomItemOfTier(ItemTier::Legendary));
				continue;
			}
			if (chanceHit <= currentTierConfig["epicChance"]) {
				itemsToSpawn.push_back(context.ir->GetRandomItemOfTier(ItemTier::Epic));
				continue;
			}
			if (chanceHit <= currentTierConfig["rareChance"]) {
				itemsToSpawn.push_back(context.ir->GetRandomItemOfTier(ItemTier::Rare));
				continue;
			}
			itemsToSpawn.push_back(context.ir->GetRandomItemOfTier(ItemTier::Common));
		}


		// present items to player (currently all random)
		for (auto item : itemsToSpawn) {
			if (item.id == 0) continue;
			auto newItem = new Item(item.data);
			newItem->SetBasePos(ctx.source->GetPosition());
			context.currentScene->AddElement(newItem);
		}
		
		ctx.source->RemoveItem(4, 999999); // remove all (hopefully)
		ctx.source->AddItem(4, 1);
	}
}
