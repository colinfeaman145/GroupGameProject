#include "Bullet.hpp"
#include "GameContext.hpp"
#include "AnimatedSprite.hpp"
#include "Attackable.hpp"


Bullet::Bullet(std::string spritePath) {
		SDL_Texture* bulletTexture = context.txm->LoadTexture(context.renderer, spritePath);
		bulletSprite = new AnimatedSprite();
		bulletSprite->Initialize(bulletTexture, 6, 6, 0, 0, 50, 50, 2, 2);
		bulletSprite->SetDrawLayer(RenderLayer::PLAYER);
		bulletSprite->SetFrameDuration(0.1);
		bulletSprite->SetLooping(true);
		bulletSprite->SetLeaveOnLastFrame(false);
}

bool Bullet::Initialize(EventContext ctx, int pierceCount, float ttl, float bulletSpeed, float rotation, AnimatedSprite* spr) {
	Entity::Initialize(ctx.source->GetPosition(), bulletSprite);

	velocity = (ctx.targetPosition - ctx.source->GetPosition()).Normalized() * bulletSpeed;

	collisionBound = CollisionShape::MakeCircle(10);//make bullet hitbox a circle with radius 10, can be changed
	collideType = CollidableType::ENEMY;

	damage = ctx.hitInfo.damageDealt;
	source = ctx.source;
	this->pierceCount = pierceCount;
	this->ttl = ttl;
	this->bulletSpeed = bulletSpeed;
	bulletSprite->Animate();

	context.grid->UpdateOccupancy((Entity*)this, &GridCell::AddOther, &GridCell::RemoveOther);
	return true;

}
void Bullet::Draw(Renderer* renderer) {
	Entity::Draw(renderer);
}
void Bullet::Process(float deltaTime) {
	Entity::Process(deltaTime);
	context.grid->UpdateOccupancy((Entity*)this, &GridCell::AddOther, &GridCell::RemoveOther);

	//remove bullet if it has no pierces left
	if (pierceCount <= 0) {
		isToBeDeleted = true;
		return;
	}

	if (ttl > 0) {
		ttl -= deltaTime;
	}
	else {
		isToBeDeleted = true;
		return;
	}
}
void Bullet::HandleCollision(Collidable* other, Vector2 penetration) {
	if (dynamic_cast<Attackable*>(other) == source) {
		return;//dont hit shooting entity
	}
	if (std::find(collisions.begin(), collisions.end(), other) != collisions.end()) {
		return;//already collided with this target
	}
	if (Attackable* target = dynamic_cast<Attackable*>(other)) {
		HitInfo info{ .damageDealt = damage, .isCritical = false, .isDodged = false, .appliesOnHitEffects = true };
		source->DealDamageTo(target, info);
		collisions.push_back(other);
		pierceCount--;
	}
}

