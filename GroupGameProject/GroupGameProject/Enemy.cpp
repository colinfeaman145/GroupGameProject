#include "Enemy.hpp"
#include "Grid.hpp"
//#include "Player.hpp"
//#include "Nature.hpp"

/*

Enemy::Enemy() {}

Enemy::~Enemy() {
    delete moving;
    delete attacking;
    delete death;
    if (explosion) context.grid->RemoveOther(explosion);
    delete explosion;
}

void Enemy::Initialize(Vector2 pos, AnimatedSprite* spr, float retarget, int targetRad, float atlasTarget, float playerTarget) {
    retargetCooldown = retarget;
    currentRetargetTime = retarget;
    targetRadius = targetRad;
    adjustCourseTimer = 1.5f;
    //atlasTargetChance = max(atlasTarget, 0.0f);//must be atleast 0
    //playerTargetChance = max(playerTarget, 0.0f);//must be atleast 
    Entity::Initialize(pos, Vector2(), spr);
    collideType = CollidableType::ENEMY;
    previousPosition = pos;
    stuckTime = 0;
    frozenTime = 0;
    unfrozen = false;
    framesSinceLastHone = 0;
    movementSpeed = 35;
    explosion = nullptr;
    //target and velocity defined in process on first pass
}

void Enemy::Draw(Renderer* renderer) {
    Entity::Draw(renderer);
    if (explosion) explosion->Draw(renderer);
}

void Enemy::Process(float deltaTime) {

    if (!IsAlive()) return;

    //update timers
    currentRetargetTime -= deltaTime;
    adjustCourseTimer -= deltaTime;
    currentAttackCooldown -= deltaTime;
    frozenTime -= deltaTime;
    framesSinceLastHone++;

    //standard process
    Entity::Process(deltaTime);

    if (frozenTime > 0) {
        SetFlash(true);
        context.grid->UpdateEnemyOccupancy(this);
        return;
    }
    else if (unfrozen == false) {//on first frame unfrozen
        unfrozen = true;
        velocity = Vector2();
        Hone();
    }

    if (!attacking->IsAnimating() && framesSinceLastHone >= 5 && currentAttackCooldown <= 0) {//if walking
        Hone(); //move towards target
        sprite = moving;
        framesSinceLastHone = 0;
    }

    context.grid->UpdateEnemyOccupancy(this);
    if (velocity.x != 0)
        SetSpriteDirection(velocity.x < 0);//face correction direciton

    //check if stuck
    float distance = (GetPosition() - previousPosition).Length();
    previousPosition = GetPosition(); // update every frame
    if (distance > 1.0f)
        stuckTime = 0;
    else
        stuckTime += deltaTime;

    //check stuckTime in handleCollision
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
    alive = false;

    //spawn drops
    ResourceDrop drops = ResourceDrop();
    drops.amount = GetDropAmount();
    drops.type = GetDropType();
    drops.spawnerPosition = GetPosition();
    drops.spawnerSize = GetSprite()->GetDrawSize();
    context.grid->SpawnDrops(drops);

    SetCanCollide(false);
    moving->Pause();
    attacking->Pause();
    death->Restart();
    death->Animate();
    death->SetPosition(position.x, position.y);
    sprite = death;
}

void Enemy::Damage(float amount) {
    float damage = IsFrozen() ? amount * 2.5 : amount;
    Entity::Damage(damage);
    uniform_int_distribution<int> slimeSoundGen(1, 3);
    context.am->PlaySound("SlimeHit" + to_string(slimeSoundGen(gen)), "Default", { position.x, 100, position.y }, { 0, 0, 0 }, Vector2(0.85, 1.15));
}

bool Enemy::IsDying() {
    if (alive) return false;

    bool deathPlaying = sprite == death && death->IsAnimating();

    if (type == EnemyType::EXPLOSIVE) {
        bool explosionPlaying = static_cast<AnimatedSprite*>(explosion->GetSprite())->IsAnimating();
        return deathPlaying || explosionPlaying; //kill fully only when both are done
    }

    return deathPlaying;
}

void Enemy::SetAttackCooldown(float atckCool) {
    attackCooldown = atckCool;
    currentAttackCooldown = attackCooldown;
}

void Enemy::SetExplosion() {
    if (type != EnemyType::EXPLOSIVE) return;

    explosion = new Explosion(radius * 4, damage * 25);

}


Explosion* Enemy::GetExplosion() {
    return explosion;
}

float Enemy::GetAttackCooldown() {
    return attackCooldown;
}

void Enemy::SetKilledByPlayer() {
    killedByPlayer = true;
}

ResourceType Enemy::GetDropType() const {
    uniform_int_distribution<int> resourceGen(1, 40);
    if (resourceGen(gen) == 3) return ResourceType::WOOD;
    if (resourceGen(gen) == 7) return ResourceType::STONE;
    return ResourceType::COIN;
}

int Enemy::GetDropAmount() const {
    int scaler = (context.gameProgress * 0.25) + 1;
    if (killedByPlayer) scaler * 1.5;
    uniform_int_distribution<int> coinsGen(4, 8);
    switch (type) {
    case(EnemyType::WALL_FOCUS):
        return coinsGen(gen) * ((int)(scaler * 0.75) + 1);
        break;
    case(EnemyType::NORMAL):
        return coinsGen(gen) * (int)(scaler);
        break;
    case(EnemyType::PLAYER_FOCUS):
        return coinsGen(gen) * (int)(scaler);
        break;
    case(EnemyType::FAST):
        return coinsGen(gen) * (int)(scaler * 1.5);
        break;
    case(EnemyType::ATLAS_FOCUS):
        return coinsGen(gen) * (int)(scaler * 1.5);
        break;
    case(EnemyType::EXPLOSIVE):
        return coinsGen(gen) * (int)(scaler * 3);
        break;
    case(EnemyType::RANDOM):
        return coinsGen(gen) * (int)(scaler * 10);
        break;
    default:
        return coinsGen(gen);
    }
}

void Enemy::Attack(Attackable* a) {
    if (currentAttackCooldown > 0) return;//cant attack
    if (attacking->IsAnimating()) return;

    if (Nature* n = dynamic_cast<Nature*>(a)) {
        a->Damage(damage * 4);//fuck up nature HARD;
    }
    else {
        a->Damage(damage);//do normal damage
        if (type == EnemyType::EXPLOSIVE) TryExplode();
        if (a->GetHealth() <= 0) {//if target died, find new one
            if (a == dynamic_cast<Attackable*>(target))
                target = nullptr;
        }
    }
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
    Entity::HandleCollision(other, penetration);
    if (other->GetCollidableType() == CollidableType::ENEMY) return;//dont attack own kind

    if (IsFrozen()) return;//dont attack while frozen;
    if (other->GetCollidableType() == CollidableType::PLAYER) {//if its player
        Attack(static_cast<Player*>(other));
        SetTarget(other);
        currentRetargetTime = retargetCooldown;//reset cooldown so focus on player isnt lost
    }
    else if (stuckTime >= .5 || other == target) {//if stuck or found target
        if (other->CanCollide()) {
            if (other->GetCollidableType() == CollidableType::STRUCTURE)
                Attack(static_cast<Structure*>(other));
            else if (other->GetCollidableType() == CollidableType::NATURE)
                Attack(static_cast<Nature*>(other));
            else
                return;
        }
    }
}
*/