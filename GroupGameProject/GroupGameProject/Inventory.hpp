#pragma once
#include <unordered_map>


// the item ID refers to the item ID in the ItemRegistry.
using ItemID = uint32_t;


// a simple inventory system that tracks items and stacks by their ID. 
class Inventory {
public:
	Inventory(std::function<void()> onChangeCallback) {
		this->onChangeCallback = onChangeCallback;
	}

	void Add(ItemID itemId, int amount) {
		m_stacks[itemId] += amount;

		onChangeCallback();
		Print();
	}
	void Remove(ItemID itemId, int amount) {
		m_stacks[itemId] -= amount;
		if (m_stacks[itemId] <= 0) m_stacks.erase(itemId);

		onChangeCallback();
		Print();
	}
	int Count(ItemID itemId) const {
		auto it = m_stacks.find(itemId);
		if (it != m_stacks.end()) return it->second;
		return 0;
	}
	const auto& All() const {
		return m_stacks;
	}
	int GetCoinCount() {
		auto coinCount = Count(3);
		if (coinCount > 1) {
			return coinCount - 1;
		}
		return 0;
	}

	void Print() const {
		printf("Inventory:\n");
		for (const auto& pair : m_stacks) {
			printf("Item ID: %d, Stacks: %d\n", pair.first, pair.second);
		}
	}
	void Clear() {
		m_stacks.clear();
		onChangeCallback();
	}

private:
	std::unordered_map<ItemID, int> m_stacks;
	std::function<void()> onChangeCallback;
};

