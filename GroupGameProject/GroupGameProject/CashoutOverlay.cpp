#include "CashoutOverlay.hpp"
#include "GameContext.hpp"
#include "PercentageBar.hpp"
#include "Camera.hpp"
#include <fstream>
#include "json.hpp"
#include "Inventory.hpp"


//NOTE: input file located in data/input.json



CashoutOverlay::CashoutOverlay(int x, int y, int w, int h, Color fillColor, Color borderColor, int alpha, int borderThickness) : 
    Container(x,y,w,h,fillColor,borderColor,alpha,borderThickness){}

CashoutOverlay::~CashoutOverlay() {
    delete cashoutBar;
    cashoutBar = nullptr;
}

bool CashoutOverlay::Initialize(Inventory* inventory)
{

    auto filepath = "../../data/items.json";
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Failed to open bindings file: " << filepath << endl;
        return false;
    }
    itemData = json::parse(file);

	cashoutBar = new PercentageBar(0, 0, size.x, size.y, {252, 194, 0, 255}, {150, 50, 50, 255}, RenderLayer::UI);
    cashoutBar->SetOffset(0, 0);

    this->inventory = inventory;
    this->inventory->RegisterCallback([this]() { this->RecalculateCashoutProgress();});
    RecalculateCashoutProgress();
    return true;
}

void CashoutOverlay::Draw(Renderer* renderer) {
    Container::Draw(renderer);

	cashoutBar->SetPosition(GetPosition().x, GetPosition().y);
    cashoutBar->Draw(renderer);
}

void CashoutOverlay::RecalculateCashoutProgress() {
    int maxCashout = 0;
    auto cashoutItemData = context.ir->Get(4).data;
    auto cashoutTiers = cashoutItemData["params"]["cashoutTiers"];
    for (auto& cashoutTier : cashoutTiers) {
        maxCashout += cashoutTier["threshold"];
    }
    cashoutBar->SetValues(inventory->Count(4) - 1, maxCashout);
}
