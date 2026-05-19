#ifndef ATTACKABLE_HPP
#define ATTACKABLE_HPP

#include "Vector2.hpp"
#include "Collidable.hpp"
#include "Entity.hpp"
#include "PercentageBar.hpp"
#include <random>

//A living thing
class Attackable : public Entity {
	public:
		void Process(float deltaTime);
		void Draw(Renderer* renderer);

		int GetHealth();
		int GetMaxHealth();
		void SetHealth(float h);
		void Damage(float amount);
		void Heal(float amount);
		void SetPosition(Vector2 pos) override;

		PercentageBar* GetHealthBar() const { return healthBar; };
		bool IsAlive() const { return isAlive; };

		void SetFlash(bool flash);

	protected:
		PercentageBar* healthBar;
		float maxHealth;
		float health;
		bool isAlive;

		float flashDuration;
};

#endif