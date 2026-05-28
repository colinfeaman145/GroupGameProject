#pragma once
#include "Entity.hpp"
#include "json.hpp"

using ItemID = uint32_t;
using json = nlohmann::json;


class Item : public Entity {
public:
	Item(json itemData);

	void Draw(Renderer* renderer) override;
	void Process(float deltaTime) override;
	void HandleCollision(Collidable* other, Vector2 penetration) override;
	void SetBasePos(Vector2 pos);

public:
	ItemID id;
private:

	// wobble stats
	float m_fAmplitude = 10;
	float m_fFrequency = 5;
	float m_fSinYPos = 0;
	Vector2 basePos;
};


