#include "Collidable.hpp"
#include "AI.hpp"
#include "EnumTypes.hpp"
#include "Renderer.hpp"

Collidable::~Collidable() {
    ClearTargetedBy();
}

void Collidable::ClearTargetedBy() {
    vector<AI*> copy = targetedBy;
    for (AI* t : copy)
        t->SetTarget(nullptr);
    targetedBy.clear();
}

Vector2 Collidable::GetCenter() const {
    Vector2 wp = collisionBound.WorldPosition(GetPosition());
    switch (collisionBound.type) {
    case ShapeType::Circle:
    case ShapeType::Cone:
        return wp;
    case ShapeType::Square:
    default:
        return {
            wp.x + (collisionBound.box.Width() * 0.5f),
            wp.y + (collisionBound.box.Height() * 0.5f)
        };
    }
}

void Collidable::Draw(Renderer* renderer, Color c, int a, RenderLayer layer) {
    CollisionShape s = GetCollisionBound();
    Vector2 wp = s.WorldPosition(GetPosition());
    switch (s.type) {
    case ShapeType::Square:
        renderer->AddDrawRect(wp.x, wp.y, s.box.width, s.box.height, c, layer);
        break;
    case ShapeType::Circle: {
        renderer->AddDrawCircle(wp.x, wp.y, s.radius, c, layer);
        break;
    }
    case ShapeType::Cone:
        renderer->AddDrawCone(wp.x, wp.y, s.radius, s.direction, s.halfAngle, c, layer);
        break;
    }
}