#include "StatSheetOverlay.hpp"
#include <format>
#include "StatSheet.hpp"
#include "Camera.hpp"
#include "Inventory.hpp"
#include "GameContext.hpp"
#include "Sprite.hpp"
#include "Text.hpp"


StatSheetOverlay::StatSheetOverlay(int x, int y, int w, int h, Color fillColor, Color borderColor, int alpha, int borderThickness) :
	Container(x, y, w, h, fillColor, borderColor, alpha, borderThickness) {
}


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

	// Build sprites and text objects once; values are updated each frame in Process
	healthText = nullptr;
	regenText = nullptr;
	damageText = nullptr;
	attackSpeedText = nullptr;
	speedText = nullptr;
	critChanceText = nullptr;
	armorText = nullptr;
	RecalculateStatSheet();
	return true;
}

void StatSheetOverlay::Draw(Renderer* renderer) {
	Container::Draw(renderer);
}

void StatSheetOverlay::Process(float deltaTime) {
	Container::Process(deltaTime);
	// Only update text values each frame, no reallocation
	UpdateStatValues();
}


// Creates all stat row sprites and stores Text pointers for later value updates.
// Should only be called once — use UpdateStatValues() every frame instead.
void StatSheetOverlay::RecalculateStatSheet() {
	Clear();

	healthText = CreateStatRow("../../assets/sprites/Items/item688.png", { 0, 0 });
	regenText = CreateStatRow("../../assets/sprites/Items/item915.png", { 0, rowHeight });
	damageText = CreateStatRow("../../assets/sprites/Items/item907.png", { 0, rowHeight * 2 });
	attackSpeedText = CreateStatRow("../../assets/sprites/Items/item908.png", { 0, rowHeight * 3 });
	speedText = CreateStatRow("../../assets/sprites/Items/item267.png", { 0, rowHeight * 4 });
	critChanceText = CreateStatRow("../../assets/sprites/Items/item1219.png", { 0, rowHeight * 5 });
	armorText = CreateStatRow("../../assets/sprites/Items/item199.png", { 0, rowHeight * 6 });

	// Populate initial values
	UpdateStatValues();
}

// Updates the text values each frame without reallocating any objects
void StatSheetOverlay::UpdateStatValues() {
	if (!statSheet) return;

	healthText->SetText(std::format("{:.2f}", statSheet->GetFinalHealth()));
	regenText->SetText(std::format("{:.2f}/s", statSheet->regernation));
	damageText->SetText(std::format("{:.2f}", statSheet->GetFinalDamage()));
	attackSpeedText->SetText(std::format("{:.2f}", statSheet->GetFinalAttackSpeed()));
	speedText->SetText(std::format("{:.2f}", statSheet->GetFinalSpeed()));
	critChanceText->SetText(std::format("{:.2f}%", statSheet->critChance * 100));
	armorText->SetText(std::to_string(statSheet->armor));
}

// Creates a stat row with an icon sprite and an empty Text object.
// Returns the Text pointer so the caller can store it for per-frame updates.
Text* StatSheetOverlay::CreateStatRow(const std::string& statIconPath, Vector2 pos) {
	auto statValue = new Text();
	statValue->Initialize("", "../../assets/fonts/pixelpurl/PixelPurl.ttf", rowHeight);
	statValue->SetColor({ 255, 255, 255, foregroundAlpha });

	SDL_Texture* itemTexture = context.txm->LoadTexture(context.renderer, statIconPath);
	auto statSprite = new Sprite();
	statSprite->Initialize(itemTexture, 16, 16, 0, 0, rowHeight, rowHeight);
	statSprite->SetDrawLayer(RenderLayer::PLAYER);
	statSprite->SetColor({ 255, 255, 255, foregroundAlpha });

	AddSprite(statSprite, pos.x, pos.y);
	AddSprite(statValue, pos.x + rowHeight + 5, pos.y);

	return statValue;
}

void StatSheetOverlay::Clear() {
	for (auto& item : sprites) {
		delete item.sprite;
		item.sprite = nullptr;
	}
	sprites.clear();

	// Nullify text pointers — memory is owned by sprites list above
	healthText = regenText = damageText = attackSpeedText = nullptr;
	speedText = critChanceText = armorText = nullptr;
}