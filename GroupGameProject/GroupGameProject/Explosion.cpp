#include "Explosion.hpp"
#include "AnimatedSprite.hpp"
#include "GameContext.hpp"

Explosion::Explosion(const Explosion& other)
	: Explosion(other.radius * 2, other.damage, other.onPlayersTeam) {
}

Explosion::Explosion(int size, int dam, bool playersExplosion) {

	AnimatedSprite* s = new AnimatedSprite();
	SDL_Texture* tex = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Explosions/explosion.png");
	s->Initialize(tex, 125, 125, 0, 0, size, size, 4, 16);
	s->SetFrameDuration(0.15);
	s->SetLooping(false);
	s->SetLeaveOnLastFrame(false);
	SetCanCollide(false);
	sprite = s;
	radius = (float)size / 2;
	position = Vector2();
	velocity = Vector2();

	collisionBound = CollisionShape::MakeCircle(radius * 1.2);

	damage = dam;
	onPlayersTeam = playersExplosion;

	canDamage = false;
	activated = false;
	damageDelay = 0.3;//2ish frames
	currentTimer = 0;
}

void Explosion::Process(float deltaTime) {
	if (activated) {
		currentTimer += deltaTime;
		if (currentTimer >= damageDelay && !damageDealt) {
			canDamage = true;
		}
	}

	if (canDamage) {
		context.grid->ResolveCollisions(this);
		canDamage = false;
		damageDealt = true;
	}

	AnimatedSprite* a = static_cast<AnimatedSprite*>(sprite);
	if (!a->IsAnimating()) {
		activated = false;
		currentTimer = 0;
	}
	Entity::Process(deltaTime);
}

void Explosion::Explode() {
	activated = true;
	damageDealt = false;
	static_cast<AnimatedSprite*>(sprite)->Restart();
	static_cast<AnimatedSprite*>(sprite)->Animate();
}

//center around pos
void Explosion::SetPosition(Vector2 pos) {
	position = Vector2(pos.x - radius, pos.y - radius);
	sprite->SetPosition(position);
	context.grid->UpdateOccupancy(static_cast<Entity*>(this), &GridCell::AddOther, &GridCell::RemoveOther);
}

bool Explosion::isActive() {
	return activated;
}

float Explosion::GetDamageScaler(Collidable* c) {
	float dist = Distance(position, c->GetPosition());
	return (dist / radius) * 1.25;
}

void Explosion::IncreaseDamage(float amount) {
	damage += amount;
}

void Explosion::HandleCollision(Collidable* other, Vector2 penetration) {
	if (canDamage) {
		if ((onPlayersTeam && other->GetCollidableType() == CollidableType::ENEMY) ||
		   (!onPlayersTeam && other->GetCollidableType() == CollidableType::PLAYER)) {
			HitInfo info;
			info.damageDealt = damage * GetDamageScaler(other);
			info.appliesOnHitEffects = true;
			DealDamageTo(static_cast<Attackable*>(other), info);
		}
	}
}