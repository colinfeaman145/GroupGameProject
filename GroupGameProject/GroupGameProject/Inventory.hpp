#pragma once
#include <unordered_map>
#include <functional>


// the item ID refers to the item ID in the ItemRegistry.
using ItemID = uint32_t;


// a simple inventory system that tracks items and stacks by their ID. 
class Inventory {
public:
	Inventory() {}

	void RegisterCallback(std::function<void()> func) {
		onChangeCallbacks.push_back(func);
	}

	void Add(ItemID itemId, int amount) {
		m_stacks[itemId] += amount;

		EmitCallbacks();
	}
	void Remove(ItemID itemId, int amount) {
		m_stacks[itemId] -= amount;
		if (m_stacks[itemId] <= 0) m_stacks.erase(itemId);

		EmitCallbacks();
	}
	int Count(ItemID itemId) const {
		auto it = m_stacks.find(itemId);
		if (it != m_stacks.end()) return it->second;
		return 0;
	}
	const auto& All() const {
		return m_stacks;
	}

	void Print() const {
		printf("Inventory:\n");
		for (const auto& pair : m_stacks) {
			printf("Item ID: %d, Stacks: %d\n", pair.first, pair.second);
		}
	}
	void Clear() {
		m_stacks.clear();
		EmitCallbacks();
	}
	void EmitCallbacks() {
		for (auto callback : onChangeCallbacks) {
			callback();
		}
	}

private:
	std::unordered_map<ItemID, int> m_stacks;
	std::vector<std::function<void()>> onChangeCallbacks;
};

