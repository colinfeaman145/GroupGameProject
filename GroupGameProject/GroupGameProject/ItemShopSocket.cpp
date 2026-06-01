#include "ItemShopSocket.hpp"
#include "Player.hpp"
#include "GameContext.hpp"
#include "Grid.hpp"
#include "Text.hpp"
#include "Item.hpp"
#include <string>

ItemShopSocket::ItemShopSocket() = default;

bool ItemShopSocket::Initialize(Vector2 pos, ItemID itemId)
{

    auto drawLayer = RenderLayer::OBJECTS;

	SDL_Texture* socketTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Items/item1128.png");
	auto socketSprite = new Sprite();
	socketSprite->Initialize(socketTexture, 16, 16, 0, 0, 100, 100);
	socketSprite->SetDrawLayer(drawLayer);
    Entity::Initialize(pos, socketSprite);
    auto currentPos = GetPosition();
    auto itemDef = context.ir->Get(itemId);

    // init shown item
    item = new Item(itemDef.data);
    item->SetBasePos({ currentPos.x - 50, currentPos.y - 170 });
    item->SetCanCollide(false);

    if (itemDef.tier == ItemTier::Common) {
        cost = 5;
    }
    else if (itemDef.tier == ItemTier::Rare) {
        cost = 10;
    }
    else if (itemDef.tier == ItemTier::Epic) {
        cost = 15;
    }
    else if (itemDef.tier == ItemTier::Legendary) {
        cost = 25;
    }
    else {
        cost = 0;
    }


    // setup label
    const std::string costStr = std::to_string(cost);
    const std::string fontPath = "../../assets/fonts/pixelpurl/PixelPurl.ttf";
    text = new Text();
    text->Initialize(costStr, fontPath, 100);
    text->SetPosition({ currentPos.x - 15, currentPos.y + 80 });
    text->SetDrawLayer(drawLayer);

    return true;
}

void ItemShopSocket::HandleCollision(Collidable* other, Vector2 penetration) {
    if (isSoldOut) return;
    if (other->GetCollidableType() != CollidableType::PLAYER) return;

    // only player can interact
    auto player = dynamic_cast<Player*>(other);
    if (player == nullptr) return;

    // only buy if player has enough coins
    auto coinCount = player->GetItemCount(3);
    if (coinCount < cost) return;

    item->SetVisibliliy(Visibility::ABSENT);
    player->AddItem(item->id, 1);
    player->RemoveItem(3, cost);
    isSoldOut = true;
}

void ItemShopSocket::Process(float deltaTime) {
    //collision updates
    Entity::Process(deltaTime);
    item->Process(deltaTime);
    text->Process(deltaTime);
}

void ItemShopSocket::Draw(Renderer* renderer) {
    Entity::Draw(renderer);
    item->Draw(renderer);
    text->Draw(renderer);
}
