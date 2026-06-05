#ifndef COLLIDABLE_HPP
#define COLLIDABLE_HPP
#include "Vector2.hpp"
#include "EnumTypes.hpp"
#include "Renderer.hpp"

class AI;
using namespace std;

enum class CollidableType {
    ENEMY,
    PLAYER,
    STRUCTURE,
    NATURE,
    RESOURCE,
    ATTACK_CONE,
    OTHER
};

struct AABB {
    float width, height;

    AABB() : width(0), height(0) {}
    AABB(float w, float h) : width(w), height(h) {}

    void UpdateSize(int w, int h) { width = w, height = h; }
    float Width()  const { return width; }
    float Height() const { return height; }
    Vector2 Center(Vector2 pos) const { return Vector2(pos.x + (Width() * 0.5), pos.y + (Height() * 0.5)); }

    Vector2 Min(Vector2 pos) const { return pos; }
    Vector2 Max(Vector2 pos) const { return { pos.x + Width(),  pos.y + Height() }; }
};


struct CollisionShape {
    ShapeType type = ShapeType::Square;
    Vector2 offset = { 0 ,0 };
    AABB box;
    float radius = 0.0f; //for circle/cone only

    // Cone only
    Vector2 direction = { 1, 0 }; //unit vector pointing towards center of arc
    float halfAngle = PI; //half total arc in radians


    static CollisionShape MakeAABB(float w, float h, Vector2 offset = { 0, 0 }) {
        CollisionShape s;
        s.type = ShapeType::Square;
        s.offset = offset;
        s.box = AABB(w, h);
        return s;
    }

    static CollisionShape MakeCircle(float radius, Vector2 offset = { 0, 0 }) {
        CollisionShape s;
        s.type = ShapeType::Circle;
        s.offset = offset;
        s.radius = radius;
        return s;
    }

    static CollisionShape MakeCone(float radius, Vector2 direction, float halfAngle, Vector2 offset = { 0, 0 }) {
        CollisionShape s;
        s.type = ShapeType::Cone;
        s.offset = offset;
        s.radius = radius;
        s.direction = direction;
        s.halfAngle = halfAngle;
        return s;
    }

    void RotateCone(float angle) {
        float c = cos(angle);
        float s = sin(angle);
        direction = {
            direction.x * c - direction.y * s,
            direction.x * s + direction.y * c
        };
    }

    void SetConeRotation(float angle) {
        direction = { std::cos(angle), std::sin(angle) };
    }

    // World-space AABB accounting for offset and position
    Vector2 WorldPosition(Vector2 ownerPos) const {
        return ownerPos + offset;
    }

    // World-space center for circle/cone
    Vector2 AABBWorldCenter(Vector2 ownerPos) const {
        return { ownerPos.x + offset.x + (box.Width() / 2), ownerPos.y + offset.y + (box.Height() / 2) };
    }
};

class Collidable {
public:
    uint32_t seenStamp = 0;//for saving memory in GetNearbyCollidables()
protected:
    CollisionShape collisionBound;
    CollidableType collideType;
    bool canCollide; //can you go through this collidable
public:

    vector<AI*> targetedBy;
    virtual ~Collidable();
    virtual void ClearTargetedBy();
    virtual CollisionShape& GetCollisionBound() { return collisionBound; }
    virtual inline void SetCollisionBound(CollisionShape cs) { collisionBound = cs; }
    virtual bool CanCollide() const { return canCollide; }
    virtual inline void SetCanCollide(bool collide) { canCollide = collide; }
    virtual Vector2 GetPosition() const = 0;
    virtual CollidableType GetCollidableType() const { return collideType; }
    virtual Vector2 GetCenter() const;
    virtual void HandleCollision(Collidable* other, Vector2 penetration) = 0;

    //debug 
    virtual void Draw(Renderer* renderer, Color c, int a = 255, RenderLayer layer = RenderLayer::DEBUG);
};

#endif