#ifndef ATTACKABLE_HPP
#define ATTACKABLE_HPP

#include "Vector2.hpp"
#include "Collidable.hpp"
#include "Entity.hpp"
#include "PercentageBar.hpp"
#include <random>



struct StatSheet {
	// character stats are calculated as (base + bonus) * mult
	int baseHealth;
	int bonusHealth;
	float healthMult;
	
	int baseDamage;
	int bonusDamage;
	float damageMult;

	int baseSpeed;
	int bonusSpeed;
	float speedMult;

	float GetHealth() const {
		return (baseHealth + bonusHealth) * healthMult;
	}

	float GetAttack() const {
		return (baseDamage + bonusDamage) * damageMult;
	}
	float GetSpeed() const {
		return (baseSpeed + bonusSpeed) * speedMult;
	}
};


//A living thing
class Attackable : public Entity {
	public:
		bool Initialize(Vector2 pos, Sprite* spr = nullptr);
		void Process(float deltaTime);
		void Draw(Renderer* renderer);

		int GetHealth();
		int GetMaxHealth();
		void SetHealth(float h);
		void ApplyDamage(float amount);
		void Heal(float amount);
		void SetPosition(Vector2 pos);

		PercentageBar* GetHealthBar() const { return healthBar; };
		bool IsAlive() const { return isAlive; };

		void SetFlash(bool flash);

	protected:
		StatSheet* m_pStats;
		float m_fCurrentHealth;

		PercentageBar* healthBar;
		bool isAlive;

		float flashDuration;
};

#endif