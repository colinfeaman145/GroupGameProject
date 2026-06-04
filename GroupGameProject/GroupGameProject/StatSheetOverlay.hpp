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
	Text* CreateStatRow(const std::string& statIconPath, Vector2 pos);
	void UpdateStatValues();

private:
	Inventory* inventory;
	StatSheet* statSheet;
	float rowHeight;
	float foregroundAlpha;

	Text* healthText;
	Text* regenText;
	Text* damageText;
	Text* attackSpeedText;
	Text* speedText;
	Text* critChanceText;
	Text* armorText;
};

