#include "StatSheetOverlay.hpp"
#include <format>
#include "StatSheet.hpp"
#include "Camera.hpp"
#include "Inventory.hpp"
#include "GameContext.hpp"
#include "Sprite.hpp"
#include "Text.hpp"


StatSheetOverlay::StatSheetOverlay(int x, int y, int w, int h, Color fillColor, Color borderColor, int alpha, int borderThickness) : 
	Container(x,y,w,h,fillColor,borderColor, alpha, borderThickness) {}


StatSheetOverlay::~StatSheetOverlay() {
    Clear();
}

bool StatSheetOverlay::Initialize(StatSheet* statSheet, Inventory* inventory, float rowHeight, float foregroundAlpha)
{
    this->statSheet = statSheet;
    this->inventory = inventory;
	this->rowHeight = rowHeight;
	this->foregroundAlpha = foregroundAlpha;
    //inventory->RegisterCallback([this]() { this->RecalculateStatSheet();});
    RecalculateStatSheet();
	return true;
}


// recalculates the content of the inventory
void StatSheetOverlay::RecalculateStatSheet() {
    Clear();
	auto health = std::format("{:.2f}", statSheet->GetFinalHealth());
	CreateStatRow("../../assets/sprites/Items/item688.png", health, {0,0});

	auto regeneration = std::format("{:.2f}/s", statSheet->regernation);
	CreateStatRow("../../assets/sprites/Items/item915.png", regeneration, {0,rowHeight});

	auto damage = std::format("{:.2f}", statSheet->GetFinalDamage());
	CreateStatRow("../../assets/sprites/Items/item907.png", damage, {0,rowHeight * 2});

	auto attackSpeed = std::format("{:.2f}", statSheet->GetFinalAttackSpeed());
	CreateStatRow("../../assets/sprites/Items/item908.png", attackSpeed, {0,rowHeight * 3});

	auto speed = std::format("{:.2f}", statSheet->GetFinalSpeed());
	CreateStatRow("../../assets/sprites/Items/item267.png", speed, {0,rowHeight * 4});

	auto critChance = std::format("{:.2f}%", statSheet->critChance * 100);
	CreateStatRow("../../assets/sprites/Items/item1219.png", critChance, {0,rowHeight * 5});

	auto armor = std::to_string(statSheet->armor);
	CreateStatRow("../../assets/sprites/Items/item199.png", armor, {0,rowHeight * 6});
}

void StatSheetOverlay::CreateStatRow(const std::string& statIconPath, std::string& value, Vector2 pos) {
	auto statValue = new Text();
	statValue->Initialize(value, "../../assets/fonts/pixelpurl/PixelPurl.ttf", rowHeight);
	statValue->SetColor( {255,255,255,foregroundAlpha});

	SDL_Texture* itemTexture = context.txm->LoadTexture(context.renderer, statIconPath);
	auto statSprite = new Sprite();
	statSprite->Initialize(itemTexture, 16, 16, 0, 0, rowHeight, rowHeight);
	statSprite->SetDrawLayer(RenderLayer::PLAYER);
	statSprite->SetColor( {255,255,255,foregroundAlpha});
	AddSprite(statSprite, pos.x, pos.y);
	AddSprite(statValue, pos.x + rowHeight + 5, pos.y);
}

void StatSheetOverlay::Draw(Renderer* renderer) {
	Container::Draw(renderer);
}

void StatSheetOverlay::Process(float deltaTime) {
	Container::Process(deltaTime);
	RecalculateStatSheet();

}

void StatSheetOverlay::Clear() {
    for (auto& item : sprites) {
		delete item.sprite;
		item.sprite = nullptr;
    }
    sprites.clear();
}
