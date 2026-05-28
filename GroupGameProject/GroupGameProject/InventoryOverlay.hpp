#pragma once
#include <vector>
#include "Container.hpp"
#include "Renderer.hpp"

class Sprite;
class Text;
struct InventoryItem {
	Sprite* sprite;
	Text* count;
	Vector2 pos;
};


class Player;
class Inventory;
class InventoryOverlay : public Container {
public:

	InventoryOverlay(int x, int y, int w, int h, Color fillColor, Color borderColor, int alpha, int borderThickness);
	~InventoryOverlay();
	bool Initialize(Inventory* player, float cellSize);
	void Recalculate();
	void Draw(Renderer* renderer);
	void Clear();
	bool AddItem(const Sprite& sprite, int count);

private:
	void RecalculateItemPosition();
	void RecalculateInventory();

private:
	Inventory* inventory;
	Container* inventoryContainer;

	int rows;
	int collumns;
	float cellSize;

	std::vector<InventoryItem> currentInventory;

};

