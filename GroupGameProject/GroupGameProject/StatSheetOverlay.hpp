#pragma once
#include "Container.hpp"

class Inventory;
class StatSheet;
class StatSheetOverlay : public Container {
	
public:
	StatSheetOverlay(int x, int y, int w, int h, Color fillColor, Color borderColor, int alpha, int borderThickness);
	~StatSheetOverlay();
	bool Initialize(StatSheet* statSheet, Inventory* inventory, float rowHeight, float foregroundAlpha);
	void Draw(Renderer* renderer) override;
	void Process(float deltaTime) override;
	void Clear();

private:
	void RecalculateStatSheet();
	void CreateStatRow(const std::string& statIconPath, std::string& value, Vector2 pos);

private:
	Inventory* inventory;
	StatSheet* statSheet;
	float rowHeight;
	float foregroundAlpha;
};

