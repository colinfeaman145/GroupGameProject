#include "InventoryOverlay.hpp"
#include "Camera.hpp"
#include "Inventory.hpp"
#include "GameContext.hpp"
#include "Sprite.hpp"
#include "Text.hpp"


InventoryOverlay::InventoryOverlay(int x, int y, int w, int h, Color fillColor, Color borderColor, int alpha, int borderThickness) : 
	Container(x,y,w,h,fillColor,borderColor, alpha, borderThickness) {}

InventoryOverlay::~InventoryOverlay() {
    Clear();
}

bool InventoryOverlay::Initialize(Inventory* i, float cellSize, float foregroundAlpha)
{

    inventory = i;
    this->foregroundAlpha = foregroundAlpha;
    inventory->RegisterCallback([this]() { this->Recalculate();});
    rows = size.y / cellSize;
    collumns = size.x / cellSize;
    this->cellSize = cellSize;

    RecalculateInventory();
    RecalculateItemPosition();
	return true;
}

void InventoryOverlay::Recalculate() {
    Clear();
    RecalculateInventory();
    RecalculateItemPosition();
}

void InventoryOverlay::RecalculateItemPosition() {
    auto currentPos = GetPosition();
    auto currentItemIndex = 0;
    for (int i = 0; i < rows; i++) {
		for (int j = 0; j < collumns; j++) {
            if (currentItemIndex >= currentInventory.size()) break;
            Vector2 pos = {j * cellSize, i * cellSize};

            AddSprite(currentInventory[currentItemIndex].sprite, pos.x, pos.y);
            AddSprite(currentInventory[currentItemIndex].count, pos.x, pos.y + cellSize - currentInventory[currentItemIndex].count->GetHeight());


            currentItemIndex++;
        }
    }
}

// recalculates the content of the inventory
void InventoryOverlay::RecalculateInventory() {
    for (auto& [itemId, count] : inventory->All()) {
        auto item = context.ir->Get(itemId);
        if (item.tier == ItemTier::Module) continue;

        auto countText = new Text();
        countText->Initialize(std::to_string(count), "../../assets/fonts/pixelpurl/PixelPurl.ttf", cellSize/2);
        countText->SetColor({ 255,255,255,foregroundAlpha });

		SDL_Texture* itemTexture = context.txm->LoadTexture(context.renderer, item.data["spritePath"]);
		auto itemSprite = new Sprite();
		itemSprite->Initialize(itemTexture, 16, 16, 0, 0, cellSize, cellSize);
		itemSprite->SetDrawLayer(RenderLayer::PLAYER);
        itemSprite->SetColor({ 255,255,255,foregroundAlpha });

        InventoryItem uiItem = {
            .sprite = itemSprite,
            .count = countText,
            .pos = {0,0}
        };
        currentInventory.push_back(uiItem);
    }
}

void InventoryOverlay::Draw(Renderer* renderer) {
	Container::Draw(renderer);
}

void InventoryOverlay::Clear() {
    for (auto& item : currentInventory) {
        delete item.count;
        item.count = nullptr;
        delete item.sprite;
        item.sprite = nullptr;
    }
    currentInventory.clear();
    sprites.clear();
}
