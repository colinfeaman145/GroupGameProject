#include "Prop.hpp"
#include "Sprite.hpp"

bool Prop::Initialize(std::string name, Vector2 pos, Sprite* spr) {
	this->name = name;
	Attackable::Initialize(pos, spr);
	if (name != "Door") {
		SetHealth(100);
		int propWidth = sprite->GetWidth() * 0.75;
		int propHeight = sprite->GetHeight() * 0.75;
		SetCollisionBound(CollisionShape::MakeAABB(propWidth, propHeight, Vector2(-propWidth / 2, -propHeight / 2)));
	}
	sprite->SetDrawLayer(RenderLayer::NATURE);
	return true;
}

void Prop::Draw(Renderer* renderer) {
	if (name != "Door" && GetHealth() <= 0) return;
	Attackable::Draw(renderer);
}

void Prop::Process(float deltaTime) {
	if (name != "Door" && GetHealth() <= 0) return;
	Attackable::Process(deltaTime);
}

void Prop::HandleCollision(Collidable* other, Vector2 penetration) {
	Entity::HandleCollision(other, penetration);
}

void Prop::ApplyDamage(EventContext& ctx) {
	if (name == "Door") return;
	Attackable::ApplyDamage(ctx);
}
