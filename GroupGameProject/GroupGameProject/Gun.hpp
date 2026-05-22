#pragma once
#include "ItemEffect.hpp"
#include "Attackable.hpp"

class StatSheet;


class Bullet : public Entity {
public:

	bool Initialize(Attackable* source, Vector2 targetPos, AnimatedSprite* spr = nullptr) {
		Entity::Initialize(source->GetPosition(), spr);

		velocity = (targetPos - source->GetPosition()).Normalized() * 2000.f;//just a very high speed for now, can be changed
		collisionBound = CollisionShape::MakeCircle(10);//make bullet hitbox a circle with radius 10, can be changed
		collideType = CollidableType::ENEMY;
		canCollide = true;
		damage = 10.f;//base damage, can be changed
		this->source = source;
		spr->Animate();
		return true;

	}
	void Draw(Renderer* renderer) override {
		Entity::Draw(renderer);
	}
	void Process(float deltaTime) override {
		Entity::Process(deltaTime);
	}
	void HandleCollision(Collidable* other, Vector2 penetration) override {
		printf("HIT!!!!");
	}
	

private:
	Vector2 position;
	float damage;
	Attackable* source;

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
		newBullet->Initialize(ctx.source, ctx.targetPosition, bulletSprite);

		context.currentScene->AddElement(newBullet);
	}
};

