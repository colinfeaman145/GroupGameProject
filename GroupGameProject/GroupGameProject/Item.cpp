#include "Item.hpp"
#include "Sprite.hpp"
#include "GameContext.hpp"
#include "Attackable.hpp"
#include "AnimatedSprite.hpp"
#include "Grid.hpp"
#include <limits>

Item::Item(json itemData) {
	id = itemData["id"].get<ItemID>();
	//animation setup
    Sprite* itemSprite;
    SDL_Texture* itemTexture = context.txm->LoadTexture(context.renderer, itemData["spritePath"]);
	itemSprite = new Sprite();
	itemSprite->Initialize(itemTexture, 16, 16, 0, 0, 100, 100);
    itemSprite->SetDrawLayer(RenderLayer::ENEMIES);

	// make the starting wobbling position random (looks better if multiple items spawn simultaniously)
	std::uniform_real_distribution<float> dis(0, m_fAmplitude);
	m_fSinYPos = dis(gen);

	Entity::Initialize(basePos, itemSprite);
}

void Item::Draw(Renderer* renderer) {
	Entity::Draw(renderer);
}

void Item::Process(float deltaTime) {
	Entity::Process(deltaTime);
    context.grid->UpdateOccupancy((Entity*)this, &GridCell::AddOther, &GridCell::RemoveOther);
    context.grid->ResolveCollisions((Entity*)this);

	m_fSinYPos += deltaTime;

	// simlates a bobbing effect by modifying the y position with a sine wave
	auto yPos = basePos.y + (m_fAmplitude * sin(m_fFrequency * m_fSinYPos));
	SetPosition({ position.x, yPos });
}

void Item::HandleCollision(Collidable* other, Vector2 penetration) {
	// items can only be picked up from the player
	if (other->GetCollidableType() != CollidableType::PLAYER) {
		return;
	}
	if (isToBeDeleted) {
		return;
	}
	if (!canCollide) {
		return;
	}

	if (auto entity = dynamic_cast<Attackable*>(other)) {
		entity->AddItem(id, 1);
		FMOD_VECTOR pos = { GetPosition().x, 0, GetPosition().y };
		FMOD_VECTOR vel = { 0,0,0 };
		context.am->PlaySound("coin_pickup", "SFX", pos, vel, { 0.9f, 1.1f });
		context.grid->RemoveOther(this);
		isToBeDeleted = true;
	}

}
void Item::SetBasePos(Vector2 pos) {
	SetPosition(pos);
	basePos = pos;
}
