#pragma once
class StatSheet {

public:
	float GetFinalHealth() const {
		return (baseHealth + bonusHealth) * healthMult;
	}
	float GetFinalDamage() const {
		return (baseDamage + bonusDamage) * damageMult;
	}
	float GetFinalSpeed() const {
		return (baseSpeed + bonusSpeed) * speedMult;
	}
	float CalculateDamageReceived(float incomingDamage) const {
		// simple armor calculation, can be improved
		return incomingDamage * (100 / (100 + armor));
	}

	void Reset() {
		baseHealth = 100;
		bonusHealth = 0;
		healthMult = 1;
		baseDamage = 100;
		bonusDamage = 0;
		damageMult = 1;
		baseSpeed = 1000;
		bonusSpeed = 0;
		speedMult = 1;
		armor = 0;
		regernation = 5;
	}

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

	int armor;
	float regernation;
};

