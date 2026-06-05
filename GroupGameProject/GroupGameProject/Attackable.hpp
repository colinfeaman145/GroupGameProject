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
class AnimatedSprite;
class EventContext;


//A living thing
class Attackable : public Entity {
	public:
		Attackable();
		~Attackable();
		virtual bool Initialize(Vector2 pos, Sprite* spr) override;
		virtual void Process(float deltaTime) override;
		virtual void Draw(Renderer* renderer) override;

	void DealDamageTo(Attackable* target, HitInfo info);
	void ApplyDamage(EventContext& ctx);
	void ApplyHeal(EventContext& ctx);
	float GetHealthPercent() const;
	int GetUniqueStatusEffectCount();
	void HandleCollision(Collidable* other, Vector2 penetration) override;

	// getter
	bool IsAlive() const { return isAlive; };
	float GetSpeed();
	int GetHealth();
	int GetMaxHealth();
	PercentageBar* GetHealthBar() const { return healthBar; };
	int GetItemCount(ItemID id);
	bool IsDying();
	std::unordered_map<ItemID, int>  GetItems();
	vector<StatusEffect> GetStatusEffects();


		// setter
		void SetFlash(bool flash);
		void SetHealth(float h);
		void SetPosition(Vector2 pos) override;
		void SetDead();
		void SetSprites(AnimatedSprite* move, AnimatedSprite* attack, AnimatedSprite* die);
		void SetSpritesDrawSize(int size);
		void SetSpriteDirection(bool b);


	// item effects
	virtual void AddItem(ItemID id, int count);
	void RemoveItem(ItemID id, int count);
	void RecalculateStats();
	void FireEvent(EventType type, EventContext ctx);
	void ApplyStatusEffect(StatusEffect effect);
	void TickStatusEffect(float deltaTime);
	void TickRegeneration(float deltaTime);

	CollisionShape GetEffectRadiusBound() const { return effectRadiusBound; }
	void SetEffectRadiusBound(float radius, Vector2 offset = { 0, 0 });

	void LoadEntityDataFromJson(json section);

private:
	void LoadInventoryFromJson(json inventory);
	void LoadItemSpawnerSettingsFromJson(json spawner);
	void LoadStatsFromJson(json stats);
	void LoadAnimationsFromJson(json animations);

public:
	Inventory* m_inventory;
	StatSheet* m_pStats;

	int recentKillCount;//for rampage
protected:
	ItemSpawner* m_itemSpawner;

	float m_fLastStatusEffectTick;
	float m_fLastHealTick;
	float canHealTimer;
	std::vector<StatusEffect> m_activeStatusEffects;
	CollisionShape effectRadiusBound;

		PercentageBar* healthBar;
		bool isAlive;

		float flashDuration;

	AnimatedSprite* deathAnimation;
	AnimatedSprite* movingAnimation;
	AnimatedSprite* attackingAnimation;
	AnimatedSprite* idleAnimation;

	float rampageTimer;
	int rampageTotalKills;
};

#endif

