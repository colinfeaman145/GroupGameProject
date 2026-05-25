#ifndef ATTACKABLE_HPP
#define ATTACKABLE_HPP
#include <random>

#include "Entity.hpp"
#include "ItemEffect.hpp"

using ItemID = uint32_t;

class ItemSpawner;
class StatSheet;
class Inventory;
class PercentageBar;
class Sprite;


//A living thing
class Attackable : public Entity {
	public:
		virtual bool Initialize(Vector2 pos, Sprite* spr) override;
		virtual void Process(float deltaTime) override;
		virtual void Draw(Renderer* renderer) override;

		void DealDamageTo(Attackable* target, HitInfo info);
		float ApplyDamage(HitInfo info);
		void ApplyHeal(float amount);

		// getter
		bool IsAlive() const { return isAlive; };
		int GetHealth();
		int GetMaxHealth();
		PercentageBar* GetHealthBar() const { return healthBar; };

		// setter
		void SetFlash(bool flash);
		void SetHealth(float h);
		void SetPosition(Vector2 pos) override;



		// item effects
		void AddItem(ItemID id, int count);
		void RecalculateStats();
		void FireEvent(EventType type, EventContext ctx);
		void ApplyStatusEffect(StatusEffectType status, Attackable* source);
		void TickStatusEffect(float deltaTime);
		void TickRegeneration(float deltaTime);

		void LoadEntityDataFromJson(const string& section);
private:
	void LoadInventoryFromJson(json inventory);
	void LoadItemSpawnerSettingsFromJson(json spawner);
	void LoadStatsFromJson(json stats);

	protected:
		StatSheet* m_pStats;
		Inventory* m_inventory;
		ItemSpawner* m_itemSpawner;
		float m_fLastStatusEffectTick;
		float m_fLastHealTick;
		std::vector<StatusEffect> m_activeStatusEffects;
		float m_fCurrentHealth;

		PercentageBar* healthBar;
		bool isAlive;

		float flashDuration;
};

#endif