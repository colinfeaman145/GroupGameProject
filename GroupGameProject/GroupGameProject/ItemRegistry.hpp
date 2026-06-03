#pragma once
#include <random>
#include <string>
#include <vector>
#include "Item.hpp"

class ItemEffect;
enum ItemTier {
	Module,
	Utility,
	Common,
	Rare,
	Epic,
	Legendary
};

using ItemID = uint32_t;
struct ItemDef {
	ItemID id;
	ItemTier tier;
	ItemEffect* effect;
	json data;
};

class ItemRegistry{

public:
	void RegisterItem(const ItemDef& item) {
		// Add item to registry
		m_registeredItems.push_back(item);
	}
	ItemDef GetRandomItemOfTier(ItemTier tier) const {
		std::vector<ItemDef> itemsOfTier;
		for (auto& item : m_registeredItems) {
			if (item.tier == tier) {
				itemsOfTier.push_back(item);
			}
		}

		if (itemsOfTier.size() == 0) {
			return { 0, ItemTier::Common, nullptr }; // Return a default item if not found
		}
		else if (itemsOfTier.size() == 1) {
			return itemsOfTier[0];
		}
		else {

			std::mt19937 gen(random_device{}());
			std::uniform_int_distribution<int> dist(0, itemsOfTier.size() - 1);
			auto randIndex = dist(gen);
			return itemsOfTier[randIndex];
		}
	}
	ItemDef Get(ItemID id) const {
		for (const auto& item : m_registeredItems) {
			if (item.id == id) {
				return item;
			}
		}
		return { 0, ItemTier::Common, nullptr }; // Return a default item if not found
	}

private:
	std::vector<ItemDef> m_registeredItems;


};

