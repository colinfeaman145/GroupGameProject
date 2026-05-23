#pragma once
#include "ItemEffect.hpp"
#include "Attackable.hpp"
#include "Grid.hpp"
#include "Player.hpp"

class StatSheet;


class Bullet : public Entity {
public:

	bool Initialize(EventContext ctx, int pierceCount, float ttl, AnimatedSprite* spr = nullptr) {
		Entity::Initialize(ctx.source->GetPosition(), spr);

		velocity = (ctx.targetPosition - ctx.source->GetPosition()).Normalized() * 2000.f;//just a very high speed for now, can be changed

		collisionBound = CollisionShape::MakeCircle(10);//make bullet hitbox a circle with radius 10, can be changed
		collideType = CollidableType::ENEMY;
		canCollide = true;

		damage = ctx.hitInfo.damageDealt;
		source = ctx.source;
		this->pierceCount = pierceCount;
		this->ttl = ttl;
		spr->Animate();

		context.grid->UpdateOccupancy((Entity*)this, &GridCell::AddOther, &GridCell::RemoveOther);
		return true;

	}
	void Draw(Renderer* renderer) override {
		Entity::Draw(renderer);
	}
	void Process(float deltaTime) override {
		Entity::Process(deltaTime);

		//remove bullet if it has no pierces left
		if (pierceCount <= 0) {
			context.grid->RemoveOther((Entity*)this);
			isToBeDeleted = true;
			return;
		}
		context.grid->UpdateOccupancy((Entity*)this, &GridCell::AddOther, &GridCell::RemoveOther);
		context.grid->ResolveCollisions(this);

		if (ttl > 0) {
			ttl -= deltaTime;
		}
		else {
			context.grid->RemoveOther((Entity*)this);
			isToBeDeleted = true;
			return;
		}
	}
	void HandleCollision(Collidable* other, Vector2 penetration) override {
		if (dynamic_cast<Player*>(other)) {
			return;//dont hit player
		}
		if (Attackable* target = dynamic_cast<Attackable*>(other)) {
			HitInfo info{ .damageDealt = damage, .isCritical = false, .isDodged = false };
			source->DealDamageTo(target, info);
			pierceCount--;
		}
	}
	

private:
	float damage;
	Attackable* source;
	int pierceCount; //for piercing bullets, can be implemented later
	float ttl;

};


class Gun : public ItemEffect {
	void OnPickup(Attackable* owner, int stacks) {
	}

	void OnRemove(Attackable* owner, int stacks) {
	}

	void OnModifyStats(StatSheet& stats, int stacks) {

	}

	void OnEvent(EventType type, EventContext ctx, int stacks) {
		if (type != EventType::OnAttack) {
			return;
		}


		SDL_Texture* playerRunning = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Bullets/SMG_Pistols_bullets.png");
		auto bulletSprite = new AnimatedSprite();
		bulletSprite->Initialize(playerRunning, 6, 6, 0, 0, 50, 50, 2,2);
		bulletSprite->SetDrawLayer(RenderLayer::PLAYER);
		bulletSprite->SetFrameDuration(0.1);
		bulletSprite->SetLooping(true);
		bulletSprite->SetLeaveOnLastFrame(false);
		bulletSprite->SetRotation(atan2(ctx.targetPosition.y - ctx.source->GetPosition().y, ctx.targetPosition.x - ctx.source->GetPosition().x) * 180 / PI);


		auto newBullet = new Bullet();
		newBullet->Initialize(ctx, 1, 5, bulletSprite);

		context.currentScene->AddElement(newBullet);
	}
};

