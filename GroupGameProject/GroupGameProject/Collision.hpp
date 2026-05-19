#ifndef COLLISION_HPP
#define COLLISION_HPP
#pragma once
#include "Collidable.hpp"
#include <cmath>
#include <algorithm>

//NOTE AI helped me ALOT with this class
//I still reviewed, annotated, and modified it accordingly
namespace Collision {

    inline bool AABBvsAABB(const CollisionShape& a, Vector2 aPos, const CollisionShape& b, Vector2 bPos, Vector2& outPen) {

        Vector2 aWorld = a.WorldPosition(aPos);
        Vector2 bWorld = b.WorldPosition(bPos);

        Vector2 aMin = a.box.Min(aWorld), aMax = a.box.Max(aWorld);
        Vector2 bMin = b.box.Min(bWorld), bMax = b.box.Max(bWorld);

        float overlapX = min(aMax.x, bMax.x) - max(aMin.x, bMin.x); //rightX of left box - leftX of right box
        float overlapY = min(aMax.y, bMax.y) - max(aMin.y, bMin.y); //bottomY of top box - topY of bottom box

        //positive if overlapping
        if (overlapX <= 0 || overlapY <= 0) return false;

        Vector2 aC = a.box.Center(aWorld);
        Vector2 bC = b.box.Center(bWorld);

        if (overlapX < overlapY)//choose cheapest overlap path
            outPen = { aC.x < bC.x ? -overlapX : overlapX, 0 };
        else
            outPen = { 0, aC.y < bC.y ? -overlapY : overlapY };

        return true;
    }

    inline bool CircleVsAABB(const CollisionShape& circle, Vector2 circlePos, const CollisionShape& box, Vector2 boxPos, Vector2& outPen) {

        Vector2 center = circle.WorldPosition(circlePos);
        Vector2 bWorld = box.WorldPosition(boxPos);

        Vector2 bMin = box.box.Min(bWorld);
        Vector2 bMax = box.box.Max(bWorld);

        //nearest point on AABB to circle center
        float cx = clamp(center.x, bMin.x, bMax.x);
        float cy = clamp(center.y, bMin.y, bMax.y);

        //distance from center to nearest point on box
        float dx = center.x - cx;
        float dy = center.y - cy;
        float distSq = dx * dx + dy * dy;

        if (distSq >= circle.radius * circle.radius) return false; //circle is not colliding with box

        float dist = sqrt(distSq);
        if (dist == 0.0f) { //circle center is inside box, oh shit, fuck
            float leftDist = center.x - bMin.x;
            float rightDist = bMax.x - center.x;
            float topDist = center.y - bMin.y;
            float bottomDist = bMax.y - center.y;

            float minDist = min({ leftDist, rightDist, topDist, bottomDist });

            if (minDist == leftDist) outPen = { -(leftDist + circle.radius), 0 };
            else if (minDist == rightDist) outPen = { rightDist + circle.radius, 0 };
            else if (minDist == topDist) outPen = { 0, -(topDist + circle.radius) };
            else outPen = { 0, bottomDist + circle.radius };
            return true;
        }

        outPen = { dx / dist * (circle.radius - dist), dy / dist * (circle.radius - dist) }; //idk dude
        return true;
    }

    inline bool CircleVsCircle(const CollisionShape& a, Vector2 aPos, const CollisionShape& b, Vector2 bPos, Vector2& outPen) {

        Vector2 ac = a.WorldPosition(aPos);
        Vector2 bc = b.WorldPosition(bPos);

        float dx = ac.x - bc.x;
        float dy = ac.y - bc.y;
        float distSq = dx * dx + dy * dy;
        float radSum = a.radius + b.radius;

        if (distSq >= radSum * radSum) return false;

        float dist = sqrt(distSq);
        if (dist == 0.0f) { outPen = { radSum, 0 }; return true; }//of fuck, circles ontop of eachother, what? Get out

        outPen = { dx / dist * (radSum - dist), dy / dist * (radSum - dist) };
        return true;
    }

    //contactDir = vector of contact point
    //dot(a, b) = cos(angle between them)
    //check if that angle is smaller than cone angle(check bigger because angle = smaller when cos)
    inline bool InCone(Vector2 contactDir, Vector2 coneDir, float halfAngle) {
        float dot = contactDir.x * coneDir.x + contactDir.y * coneDir.y;
        return dot > cos(halfAngle);
    }

    inline bool ConeVsAABB(const CollisionShape& cone, Vector2 conePos, const CollisionShape& box, Vector2 boxPos, Vector2& outPen) {

        Vector2 center = cone.WorldPosition(conePos);
        Vector2 bWorld = box.WorldPosition(boxPos);

        Vector2 bMin = box.box.Min(bWorld);
        Vector2 bMax = box.box.Max(bWorld);

        //full circle test first
        CollisionShape fullCircle = cone;
        fullCircle.type = ShapeType::Circle;
        if (!CircleVsAABB(fullCircle, conePos, box, boxPos, outPen)) return false;//sets outpen. Only used if also inside cone

        //check if cone center is inside the box
        if (center.x >= bMin.x && center.x <= bMax.x &&
            center.y >= bMin.y && center.y <= bMax.y)
            return true;

        //check all 4 corners of the AABB
        Vector2 corners[4] = {
            bMin,
            { bMax.x, bMin.y },
            bMax,
            { bMin.x, bMax.y }
        };

        for (const Vector2& corner : corners) {
            float dx = corner.x - center.x;
            float dy = corner.y - center.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist == 0.0f) return true;
            if (dist <= cone.radius) {
                Vector2 contactDir = { dx / dist, dy / dist };
                if (InCone(contactDir, cone.direction, cone.halfAngle))
                    return true;
            }
        }

        //check nearest point on each edge. Needed only if cone hits 0 corners
        struct Edge { Vector2 a, b; };
        Edge edges[4] = {
            { corners[0], corners[1] },  // top
            { corners[1], corners[2] },  // right
            { corners[2], corners[3] },  // bottom
            { corners[3], corners[0]}   // left
        };

        for (const Edge& e : edges) {

            //nearest point on segment to cone center
            float edgeDx = e.b.x - e.a.x;
            float edgeDy = e.b.y - e.a.y;
            float lenSq = edgeDx * edgeDx + edgeDy * edgeDy;
            float t = 0.0f;
            if (lenSq > 0.0f)
                t = clamp(((center.x - e.a.x) * edgeDx + (center.y - e.a.y) * edgeDy) / lenSq, 0.0f, 1.0f);
            //t is a clamped value between 0 - 1 describing the position of the closest point on the edge

            float px = e.a.x + t * edgeDx;
            float py = e.a.y + t * edgeDy;
            float dx = px - center.x;
            float dy = py - center.y;
            float dist = sqrt(dx * dx + dy * dy);

            if (dist == 0.0f || dist > cone.radius) continue;
            Vector2 contactDir = { dx / dist, dy / dist };
            if (InCone(contactDir, cone.direction, cone.halfAngle))
                return true;
        }

        return false;
    }

    inline bool ConeVsCircle(const CollisionShape& cone, Vector2 conePos, const CollisionShape& circle, Vector2 circlePos, Vector2& outPen) {

        CollisionShape fullCircle = cone;
        fullCircle.type = ShapeType::Circle;
        if (!CircleVsCircle(fullCircle, conePos, circle, circlePos, outPen)) return false; //sets outpen. Only used if also inside cone

        Vector2 cc = cone.WorldPosition(conePos);
        Vector2 cr = circle.WorldPosition(circlePos);

        float dx = cr.x - cc.x;
        float dy = cr.y - cc.y;
        float dist = sqrt(dx * dx + dy * dy);

        if (dist == 0.0f) return true;

        Vector2 contactDir = { dx / dist, dy / dist };
        return InCone(contactDir, cone.direction, cone.halfAngle);
    }

    // penetration always points FROM b INTO a
    inline bool TestShapes(const CollisionShape& a, Vector2 aPos, const CollisionShape& b, Vector2 bPos, Vector2& outPen) {

        //AABB vs x
        if (a.type == ShapeType::Square && b.type == ShapeType::Square)
            return AABBvsAABB(a, aPos, b, bPos, outPen);

        else if (a.type == ShapeType::Square && b.type == ShapeType::Circle) {
            bool hit = CircleVsAABB(b, bPos, a, aPos, outPen);
            outPen = { -outPen.x, -outPen.y };
            return hit;
        }
        else if (a.type == ShapeType::Square && b.type == ShapeType::Cone) {
            bool hit = ConeVsAABB(b, bPos, a, aPos, outPen);
            outPen = { -outPen.x, -outPen.y };
            return hit;
        }

        //circle vs x
        if (a.type == ShapeType::Circle && b.type == ShapeType::Square)
            return CircleVsAABB(a, aPos, b, bPos, outPen);

        if (a.type == ShapeType::Circle && b.type == ShapeType::Circle)
            return CircleVsCircle(a, aPos, b, bPos, outPen);

        if (a.type == ShapeType::Circle && b.type == ShapeType::Cone) {
            bool hit = ConeVsCircle(b, bPos, a, aPos, outPen);
            outPen = { -outPen.x, -outPen.y };
            return hit;
        }

        //cone vs x
        if (a.type == ShapeType::Cone && b.type == ShapeType::Square)
            return ConeVsAABB(a, aPos, b, bPos, outPen);

        if (a.type == ShapeType::Cone && b.type == ShapeType::Circle)
            return ConeVsCircle(a, aPos, b, bPos, outPen);

        if (a.type == ShapeType::Cone && b.type == ShapeType::Cone) {
            //Should never happen, so I didnt write any code for it 
            CollisionShape fa = a; fa.type = ShapeType::Circle;
            CollisionShape fb = b; fb.type = ShapeType::Circle;
            return CircleVsCircle(fa, aPos, fb, bPos, outPen);
        }

        return false;
    }

}

#endif