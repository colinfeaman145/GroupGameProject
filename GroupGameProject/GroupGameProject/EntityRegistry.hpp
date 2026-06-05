#pragma once
#include <random>
#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

class ItemEffect;
enum EntityType {
	PLAYER,
	GENERIC,
	BOSS	
};

using EntityID = uint32_t;
struct EntityDef {
	EntityID id;
	EntityType type;
	json data;
};

class EntityRegistry {

public:
	void RegisterEntity(const EntityDef& item) {
		// Add item to registry
		m_registeredEnemies.push_back(item);
	}
	EntityDef GetRandomEntityOfType(EntityType type) const {
		std::vector<EntityDef> entityOfType;
		for (auto& entity : m_registeredEnemies) {
			if (entity.type == type) {
				entityOfType.push_back(entity);
			}
		}

		if (entityOfType.size() == 0) {
			return { 0, EntityType::GENERIC, nullptr }; // Return a default Entity if not found
		}
		else if (entityOfType.size() == 1) {
			return entityOfType[0];
		}
		else {

			std::mt19937 gen(std::random_device{}());
			std::uniform_int_distribution<int> dist(0, entityOfType.size() - 1);
			auto randIndex = dist(gen);
			return entityOfType[randIndex];
		}
	}
	EntityDef Get(EntityID id) const {
		for (const auto& entity : m_registeredEnemies) {
			if (entity.id == id) {
				return entity;
			}
		}
		return { 0, EntityType::GENERIC, nullptr }; // Return a default item if not found
	}

private:
	std::vector<EntityDef> m_registeredEnemies;


};
