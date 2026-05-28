#pragma once
#include "Entity.hpp"

using ItemID = uint32_t;

class Text;
class Item;
class ItemShopSocket : public Entity {
public:
	ItemShopSocket();
	virtual bool Initialize(Vector2 pos, ItemID item);
	void HandleCollision(Collidable* other, Vector2 penetration) override;
	void Process(float deltaTime) override;
	void Draw(Renderer* renderer) override;

private:
	Item* item;
	int cost;
	Text* text;
	bool isSoldOut;
};

