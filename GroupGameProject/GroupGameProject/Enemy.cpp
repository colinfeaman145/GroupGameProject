#include "Enemy.hpp"
#include "Grid.hpp"


Enemy::Enemy() {}

Enemy::~Enemy() {
    delete moving;
    delete attacking;
    delete death;
}

void Enemy::Initialize(Vector2 pos, AnimatedSprite* spr, float retarget, int targetRad, float atlasTarget, float playerTarget) {

    Attackable::Initialize(pos, spr);
    collideType = CollidableType::ENEMY;
	spr->Animate();
}

void Enemy::Draw(Renderer* renderer) {
    Attackable::Draw(renderer);
}

void Enemy::Process(float deltaTime) {

    if (!IsAlive()) return;

    //standard process
    Attackable::Process(deltaTime);
}

void Enemy::SetSprites(AnimatedSprite* move, AnimatedSprite* attack, AnimatedSprite* die) {
    moving = static_cast<AnimatedSprite*>(move->Clone());
    attacking = static_cast<AnimatedSprite*>(attack->Clone());
    death = static_cast<AnimatedSprite*>(die->Clone());
    moving->Animate();
}

void Enemy::SetSpritesDrawSize(int size) {
    moving->SetDrawSize(size, size);
    attacking->SetDrawSize(size, size);
    death->SetDrawSize(size, size);

    radius = size / 4;
    collisionBound = CollisionShape::MakeCircle(radius, Vector2(radius, radius));
    SetCanCollide(true);
}

void Enemy::SetSpriteDirection(bool b) {
    moving->SetFlip(b);
    attacking->SetFlip(b);
    death->SetFlip(b);
}

void Enemy::SetType(EnemyType t) {
    type = t;
}

EnemyType Enemy::GetType() {
    return type;
}

void Enemy::SetDamage(int d) {
    damage = d;
}

int Enemy::GetDamage() {
    return damage;
}

void Enemy::SetDead() {
    isAlive = false;


    SetCanCollide(false);
    moving->Pause();
    attacking->Pause();
    death->Restart();
    death->Animate();
    death->SetPosition(position.x, position.y);
    sprite = death;
}


bool Enemy::IsDying() {
    if (isAlive) return false;

    bool deathPlaying = sprite == death && death->IsAnimating();
    return deathPlaying;
}

void Enemy::SetAttackCooldown(float atckCool) {
    attackCooldown = atckCool;
    currentAttackCooldown = attackCooldown;
}


float Enemy::GetAttackCooldown() {
    return attackCooldown;
}

void Enemy::SetKilledByPlayer() {
    killedByPlayer = true;
}


void Enemy::HandleCollision(Collidable* other, Vector2 penetration) {
    if (!IsAlive()) return;
    //Attackable::HandleCollision(other, penetration);
    if (other->GetCollidableType() == CollidableType::ENEMY) return;//dont attack own kind

}