#include "Explosion.hpp"
#include "AnimatedSprite.hpp"
#include "GameContext.hpp"
#include "Attackable.hpp"

Explosion::Explosion() {

}

bool Explosion::Initialize(EventContext ctx, float radius) {
	SDL_Texture* explosionTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Explosions/explosion.png");
	explosionAnimation = new AnimatedSprite();
	explosionAnimation->Initialize(explosionTexture, 125, 125, 0, 0, radius, radius, 4, 16);
	explosionAnimation->SetDrawLayer(RenderLayer::PARTICLE);
	explosionAnimation->SetFrameDuration(0.05);
	explosionAnimation->SetLooping(false);

	Entity::Initialize(ctx.targetPosition, explosionAnimation);

	SetPosition({ ctx.targetPosition.x - explosionAnimation->GetWidth() / 2, ctx.targetPosition.y - explosionAnimation->GetHeight() / 2 });

	collideType = CollidableType::ENEMY;

	damage = ctx.hitInfo.damageDealt;
	source = ctx.source;

	explosionAnimation->Animate();
	context.grid->UpdateOccupancy((Entity*)this, &GridCell::AddOther, &GridCell::RemoveOther);
	return true;
}

void Explosion::Draw(Renderer* renderer) {
	Entity::Draw(renderer);
}

void Explosion::Process(float deltaTime) {
	Entity::Process(deltaTime);
}

void Explosion::HandleCollision(Collidable* other, Vector2 penetration) {
	if (std::find(collisions.begin(), collisions.end(), other) != collisions.end()) {
		return; // already collided with this target
	}
	auto target = dynamic_cast<Attackable*>(other);
	if (target == nullptr) return;


	source->DealDamageTo(target, { damage, 0, false, false, true });
	target->ApplyStatusEffect({ StatusEffectType::Burning, 3, 0, 0, source });
	collisions.push_back(other);
}
