#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "Attackable.hpp"

class Explosion : public Attackable {
public:
	Explosion(const Explosion& other);
	Explosion(int size, int dam, bool playersExplosion);
	void Process(float deltaTime) override;
	void Explode();
	void SetPosition(Vector2 pos) override;
	bool isActive();
	float GetDamageScaler(Collidable* c);
	void IncreaseDamage(float amount);

	void HandleCollision(Collidable* other, Vector2 penetration) override;

private:
	float damage;
	bool activated;
	float damageDelay;
	float currentTimer;
	bool canDamage;
	bool damageDealt;
	bool onPlayersTeam;
};

#endif