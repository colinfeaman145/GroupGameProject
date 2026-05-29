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
		float regernation = 0,
		float critChance = 0,
		float critMultiplyer = 0,
		bool healHasCritEnabled = false
	) {
		
		defaultBaseHealth = baseHealth;
		defaultBonusHealth = bonusHealth;
		defaultHealthMult = healthMult;
		defaultBaseDamage = baseDamage;
		defaultBonusDamage = bonusDamage;
		defaultDamageMult = damageMult;
		defaultBaseSpeed = baseSpeed;
		defaultBonusDamage = bonusSpeed;
		defaultSpeedMult = speedMult;
		defaultBaseAttackSpeed = baseAttackSpeed;
		defaultBonusAttackSpeed = bonusAttackSpeed;
		defaultAttackSpeedMult = attackSpeedMult;
		defaultArmor = armor;
		defaultRegernation = regernation;
		defaultCritChance = critChance;
		defaultCritMultiplyer = critMultiplyer;
		defaultHasHealCritEnabled = hasHealCritEnabled;
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
	float CalculateDamageReceived(float incomingDamage) const {
		// simple armor calculation, can be improved
		return incomingDamage * (100.f / (100 + armor));
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
		critChance = defaultCritChance;
		critMultiplyer = defaultCritMultiplyer;
		regernation = defaultRegernation;
		hasHealCritEnabled = defaultHasHealCritEnabled;
	}


	// character stats that can be modified by items and other effects.
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
	float critChance;
	float critMultiplyer;
	float regernation;
	bool hasHealCritEnabled;


private:
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
	float defaultCritChance;
	float defaultCritMultiplyer;
	float defaultRegernation;
	bool defaultHasHealCritEnabled;
};

