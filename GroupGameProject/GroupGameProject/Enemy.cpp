#include "Enemy.hpp"
#include "Grid.hpp"
//#include "Player.hpp"
//#include "Nature.hpp"


Enemy::Enemy() {}

Enemy::~Enemy() {
    delete moving;
    delete attacking;
    delete death;
}

void Enemy::Initialize(Vector2 pos, AnimatedSprite* spr, float retarget, int targetRad, float atlasTarget, float playerTarget) {


    Entity::Initialize(pos, spr);
    collideType = CollidableType::ENEMY;

}

void Enemy::Draw(Renderer* renderer) {
    Entity::Draw(renderer);
}

void Enemy::Process(float deltaTime) {

    if (!IsAlive()) return;


    //standard process
    Entity::Process(deltaTime);

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

void Enemy::Damage(float amount) {
    float damage = amount * 2.5;
    ApplyDamage(damage);
    uniform_int_distribution<int> slimeSoundGen(1, 3);
    context.am->PlaySound("SlimeHit" + to_string(slimeSoundGen(gen)), "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
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


void Enemy::Attack(Attackable* a) {
    if (currentAttackCooldown > 0) return;//cant attack
    if (attacking->IsAnimating()) return;

    a->ApplyDamage(damage);//do normal damage

    attacking->Restart();
    attacking->Animate();
    attacking->SetPosition(position.x, position.y);
    sprite = attacking;
    velocity = Vector2();//stop moving when attacking
    currentAttackCooldown = attackCooldown;

    uniform_int_distribution<int> slimeSoundGen(1, 3);
    context.am->PlaySound("SlimeHit" + to_string(slimeSoundGen(gen)), "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
}


void Enemy::HandleCollision(Collidable* other, Vector2 penetration) {
    if (!IsAlive()) return;
    //Attackable::HandleCollision(other, penetration);
    if (other->GetCollidableType() == CollidableType::ENEMY) return;//dont attack own kind

}