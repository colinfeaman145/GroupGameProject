#include "Entity.hpp"
#include "Sprite.hpp"
#include "Renderer.hpp"
#include "Grid.hpp"

Entity::Entity() {
    sprite = nullptr;
    radius = 0;
    position = Vector2(0.0f, 0.0f);
    velocity = Vector2(0.0f, 0.0f);
    occupancy = { -1, -1, -1, -1 };

    canCollide = true;
    collisionBound = CollisionShape::MakeCircle(1);
}

Entity::~Entity() {
    delete sprite;
    sprite = nullptr;
}

bool Entity::Initialize(Vector2 pos, Sprite* spr) {
    position = pos;
    sprite = spr;

    if (sprite) {
        radius = spr->GetWidth() / 2;
        sprite->SetDrawLayer(RenderLayer::ENEMIES);

        collisionBound = CollisionShape::MakeCircle(radius); //default circle
    }

    return true;
}

void Entity::Process(float deltaTime) {
    position = position + (velocity * deltaTime);

    //stay inside grid
    float worldWidth = context.grid->GetGridWidth() * context.grid->GetCellSize();
    float worldHeight = context.grid->GetGridHeight() * context.grid->GetCellSize();

    Vector2 size = sprite->GetDrawSize();
    position.x = max(0.0f, min(position.x, worldWidth - size.x));
    position.y = max(0.0f, min(position.y, worldHeight - size.y));

    if (sprite) {
        sprite->SetPosition((int)position.x, (int)position.y);
        sprite->Process(deltaTime);

    }
}

void Entity::Draw(Renderer* renderer) {
    if (sprite) {
        sprite->Draw(renderer);
    }
   
    if (DEBUGMODE) {
        if (canCollide)
            Collidable::Draw(renderer, { 255, 0, 0 });
        else
            Collidable::Draw(renderer, { 0, 255, 0 });
    }
}

void Entity::SetPosition(Vector2 pos) {
    position = pos;
    if (sprite) sprite->SetPosition(pos);
    
}

void Entity::SetVelocity(Vector2 vel) {
    velocity = vel;
}

void Entity::Rotate(float direction) {
    if (sprite) {
        sprite->SetRotation(direction);
    }
}

float Entity::GetRadius() {
    return radius;
}

Vector2 Entity::GetFacingDirection() {
    return velocity.Normalized();
}

//returns center
Vector2 Entity::GetPosition() const {
    Vector2 center = position;
    center.x += radius;
    center.y += radius;
    return center;
}

Vector2 Entity::GetCorner() {
    return position;
}

Vector2& Entity::GetVelocity() {
    return velocity;
}

Sprite* Entity::GetSprite() {
    return sprite;
}

GridOccupancy Entity::GetOccupancy() const {
    return occupancy;
}

void Entity::SetOccupancy(GridOccupancy occ) {
    occupancy = occ;
}