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
	bool Initialize(Inventory* player, float cellSize, float foregroundAlpha);
	void Recalculate();
	void Draw(Renderer* renderer);
	void Clear();

private:
	void RecalculateItemPosition();
	void RecalculateInventory();

private:
	Inventory* inventory;
	Container* inventoryContainer;

	int rows;
	int collumns;
	float cellSize;
	float foregroundAlpha;

	std::vector<InventoryItem> currentInventory;

};

