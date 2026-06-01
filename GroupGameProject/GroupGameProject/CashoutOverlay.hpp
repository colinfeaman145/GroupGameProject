#pragma once
#include "Container.hpp"
#include "json.hpp"

using json = nlohmann::json;

class Inventory;
class PercentageBar;
class CashoutOverlay : public Container {
public:
	CashoutOverlay(int x, int y, int w, int h, Color fillColor, Color borderColor, int alpha, int borderThickness);
	~CashoutOverlay();
	bool Initialize(Inventory* inventory);
	void Draw(Renderer* renderer);
	void RecalculateCashoutProgress();

private:
	Inventory* inventory;
	PercentageBar* cashoutBar;
};

