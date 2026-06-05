#pragma once

class StatSheet {

public:
	void SetDefaultValues(
		int baseHealth = 100,
		int bonusHealth = 0,
		float healthMult = 1,
		int baseDamage = 10,
		int bonusDamage = 0,
		float damageMult = 1,
		int baseSpeed = 1000,
		int bonusSpeed = 0,
		float speedMult = 1,
		float baseAttackSpeed = 0.5,
		float bonusAttackSpeed = 0,
		float attackSpeedMult = 1,
		int armor = 0,
		int weakness = 0,
		float regernation = 0,
		float regenerateTime = 5,
		float critChance = 0,
		float critMultiplyer = 0,
		bool healHasCritEnabled = false,
		float effectRadiusScaler = 1.0
	) {
		
		defaultBaseHealth = baseHealth;
		defaultBonusHealth = bonusHealth;
		defaultHealthMult = healthMult;
		defaultBaseDamage = baseDamage;
		defaultBonusDamage = bonusDamage;
		defaultDamageMult = damageMult;
		defaultBaseSpeed = baseSpeed;
		defaultBonusDamage = bonusDamage;
		defaultSpeedMult = speedMult;
		defaultBaseAttackSpeed = baseAttackSpeed;
		defaultBonusAttackSpeed = bonusAttackSpeed;
		defaultAttackSpeedMult = attackSpeedMult;
		defaultArmor = armor;
		defaultWeakness = weakness;
		defaultRegernation = regernation;
		defaultRegenerateTime = regenerateTime;
		defaultCritChance = critChance;
		defaultCritMultiplyer = critMultiplyer;
		defaultHasHealCritEnabled = hasHealCritEnabled;
		defaultEffectRadiusScaler = effectRadiusScaler;
		Reset();
	}

	float GetFinalHealth() const {
		return (baseHealth + bonusHealth) * healthMult;
	}
	float GetFinalDamage() const {
		return (baseDamage + bonusDamage) * damageMult;
	}
	float GetFinalSpeed() const {
		return (baseSpeed + bonusSpeed) * speedMult;
	}
	float GetFinalAttackSpeed() const {
		return (baseAttackSpeed + bonusAttackSpeed) * attackSpeedMult;
	}
	float GetCurrentHealth() const {
		return currentHealth;
	}
	void SetCurrentHealth(float health) {
		currentHealth = clip(health, 0, GetFinalHealth());
	}
	float CalculateDamageReceived(float incomingDamage) const {
		// simple armor calculation, can be improved
		return incomingDamage * ((100.0f + weakness) / (100 + armor));
	}

	void Reset() {
		baseHealth = defaultBaseHealth;
		bonusHealth = defaultBonusHealth;
		healthMult = defaultHealthMult;

		baseDamage = defaultBaseDamage;
		bonusDamage = defaultBonusDamage;
		damageMult = defaultDamageMult;

		baseSpeed = defaultBaseSpeed;
		bonusSpeed = defaultBonusSpeed;
		speedMult = defaultSpeedMult;

		baseAttackSpeed = defaultBaseAttackSpeed;
		bonusAttackSpeed = defaultBonusAttackSpeed;
		attackSpeedMult = defaultAttackSpeedMult;

		armor = defaultArmor;
		weakness = defaultWeakness;
		critChance = defaultCritChance;
		critMultiplyer = defaultCritMultiplyer;
		regernation = defaultRegernation;
		regenerateTime = defaultRegenerateTime;
		hasHealCritEnabled = defaultHasHealCritEnabled;
		effectRadiusScaler = defaultEffectRadiusScaler;
	}
private:
	inline float clip(float value, float min, float max) {
		if (value < min) return min;
		else if (value > max) return max;
		else return value;
	}

public:

	// character stats that can be modified by items and other effects.
	float currentHealth;
	int baseHealth;
	int bonusHealth;
	float healthMult;
	
	int baseDamage;
	int bonusDamage;
	float damageMult;

	int baseSpeed;
	int bonusSpeed;
	float speedMult;

	float baseAttackSpeed;
	float bonusAttackSpeed;
	float attackSpeedMult;

	int armor;
	int weakness;
	float critChance;
	float critMultiplyer;
	float regernation;
	float regenerateTime;
	bool hasHealCritEnabled;

	float effectRadiusScaler;


	// base stats used to reset the statsheet to default values
	int defaultBaseHealth;
	int defaultBonusHealth;
	float defaultHealthMult;

	int defaultBaseDamage;
	int defaultBonusDamage;
	float defaultDamageMult;

	float defaultBaseSpeed;
	float defaultBonusSpeed;
	float defaultSpeedMult;

	float defaultBaseAttackSpeed;
	float defaultBonusAttackSpeed;
	float defaultAttackSpeedMult;

	int defaultArmor;
	int defaultWeakness;
	float defaultCritChance;
	float defaultCritMultiplyer;
	float defaultRegernation;
	float defaultRegenerateTime;
	bool defaultHasHealCritEnabled;
	float defaultEffectRadiusScaler;
};


