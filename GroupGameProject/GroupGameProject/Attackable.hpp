#ifndef ATTACKABLE_HPP
#define ATTACKABLE_HPP

#include "Vector2.hpp"
#include "Collidable.hpp"
#include "Entity.hpp"
#include "PercentageBar.hpp"
#include "StatSheet.hpp"
#include "Inventory.hpp"
#include <random>


//A living thing
class Attackable : public Entity {
	public:
		bool Initialize(Vector2 pos, Sprite* spr = nullptr);
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
		void RecalculateStats();
		void FireEvent(EventType type, EventContext ctx);
		void ApplyStatusEffect(StatusEffectType status, Attackable* source);
		void TickStatusEffect(float deltaTime);
		void TickRegeneration(float deltaTime);

		void LoadEntityDataFromJson(const string& section);
private:
	void LoadInventoryFromJson(json inventory);
	void LoadStatsFromJson(json stats);

	protected:
		StatSheet* m_pStats;
		Inventory* inventory;
		float m_fLastStatusEffectTick;
		float m_fLastHealTick;
		std::vector<StatusEffect> m_activeStatusEffects;
		float m_fCurrentHealth;

		PercentageBar* healthBar;
		bool isAlive;

		float flashDuration;
};

#endif