#pragma once
#include <string>
#include <vector>
#include "Item.hpp"

class ItemEffect;
enum ItemTier {
	Common,
	Uncommon,
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

