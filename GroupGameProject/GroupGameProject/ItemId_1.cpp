#include "ItemId_1.hpp"
#include "Grid.hpp"
#include "Player.hpp"
#include "ItemEffect.hpp"
#include "Attackable.hpp"
#include "GameContext.hpp"
#include "AnimatedSprite.hpp"

class Bullet : public Entity {
public:

	bool Initialize(EventContext ctx, int pierceCount, float ttl, float bulletSpeed, AnimatedSprite* spr = nullptr) {
		Entity::Initialize(ctx.source->GetPosition(), spr);

		velocity = (ctx.targetPosition - ctx.source->GetPosition()).Normalized() * bulletSpeed;//just a very high speed for now, can be changed

		collisionBound = CollisionShape::MakeCircle(10);//make bullet hitbox a circle with radius 10, can be changed
		collideType = CollidableType::ENEMY;

		damage = ctx.hitInfo.damageDealt;
		source = ctx.source;
		this->pierceCount = pierceCount;
		this->ttl = ttl;
		this->bulletSpeed = bulletSpeed;
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
		if (dynamic_cast<Attackable*>(other) == source) {
			return;//dont hit shooting entity
		}
		if (std::find(collisions.begin(), collisions.end(), other) != collisions.end()) {
			return;//already collided with this target
		}
		if (Attackable* target = dynamic_cast<Attackable*>(other)) {
			HitInfo info{ .damageDealt = damage, .isCritical = false, .isDodged = false };
			source->DealDamageTo(target, info);
			collisions.push_back(other);
			pierceCount--;
		}
	}
	

private:
	float damage;
	float ttl;
	float bulletSpeed;

	Attackable* source;
	int pierceCount; //for piercing bullets, can be implemented later

	vector<Collidable*> collisions;//to prevent multiple collisions with the same target
};



void ItemId_1::OnPickup(Attackable* owner, int stacks) {
}

void ItemId_1::OnRemove(Attackable* owner, int stacks) {
}

void ItemId_1::OnModifyStats(StatSheet& stats, int stacks) {

}

void ItemId_1::OnEvent(EventType type, EventContext ctx, int stacks) {


	if (type == EventType::OnCrit) {
		ctx.target->ApplyStatusEffect(StatusEffectType::Burning, 5.f, ctx.source);
		return;
	}

	if (type != EventType::OnAttack) {
		return;
	}

	SDL_Texture* playerRunning = context.txm->LoadTexture(context.renderer, data["params"]["bulletSprite"]);
	auto bulletSprite = new AnimatedSprite();
	bulletSprite->Initialize(playerRunning, 6, 6, 0, 0, 50, 50, 2, 2);
	bulletSprite->SetDrawLayer(RenderLayer::PLAYER);
	bulletSprite->SetFrameDuration(0.1);
	bulletSprite->SetLooping(true);
	bulletSprite->SetLeaveOnLastFrame(false);
	bulletSprite->SetRotation(atan2(ctx.targetPosition.y - ctx.source->GetPosition().y, ctx.targetPosition.x - ctx.source->GetPosition().x) * 180 / PI);


	auto newBullet = new Bullet();
	auto pierceCount = data["params"]["pierceCount"].get<float>();
	auto ttl = data["params"]["ttl"].get<float>();
	auto bulletSpeed = ItemEffect::GetLinearStackingItemValue(data["params"]["bulletSpeed"].get<float>(), data["params"]["bulletSpeedPerStack"].get<float>(), stacks);

	newBullet->Initialize(ctx, pierceCount, ttl, bulletSpeed, bulletSprite);
	context.grid->UpdateOccupancy((Entity*)newBullet, &GridCell::AddOther, &GridCell::RemoveOther);
}



