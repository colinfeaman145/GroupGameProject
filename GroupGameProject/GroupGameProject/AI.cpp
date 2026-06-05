#include "AI.hpp"
#include "Grid.hpp"
#include "Sprite.hpp"
#include "GameContext.hpp"
#include "StatSheet.hpp"


AI::AI() : 
    target(nullptr),
    adjustCourseTimer(0),
    currentRetargetTime(0),
    retargetCooldown(2.0f),
    framesSinceLastHone(0), 
    targetRadius(100), 
    previousPosition(0, 0), 
    stuckTime(0), 
    isChasing(false) {}

bool AI::Initialize(Vector2 pos, Sprite* spr) {
	Attackable::Initialize(pos, spr);
	return true;
}

void AI::Process(float deltaTime) {
    if (!IsAlive()) return;

    Attackable::Process(deltaTime);
    //update timers
    currentRetargetTime -= deltaTime;
    adjustCourseTimer -= deltaTime;
    framesSinceLastHone++;

    if (framesSinceLastHone >= 10) {//if walking
        Hone(); //move towards target
        framesSinceLastHone = 0;
    }

    //check if stuck
    float distance = (GetPosition() - previousPosition).Length();
    previousPosition = GetPosition(); //update every frame
    if (distance > 1.0f)
        stuckTime = 0;
    else
        stuckTime += deltaTime;
}

void AI::Draw(Renderer* renderer) {
    Attackable::Draw(renderer);
}

void AI::SetTarget(Collidable* c) {
    if (target) {
        auto& vec = target->targetedBy;
        auto it = std::find(vec.begin(), vec.end(), this);
        if (it != vec.end()) vec.erase(it);
    }
    target = c;
    if (target)
        target->targetedBy.push_back(this);
}

void AI::Hone() {
    if (target == nullptr) return;
    if (!isChasing) return;


    GridCoord myCell = context.grid->WorldToGrid(GetPosition());
    GridCoord targetCoord = context.grid->WorldToGrid(target->GetPosition());

    if (myCell.col == targetCoord.col && myCell.row == targetCoord.row) {
        Vector2 toTarget = target->GetCenter() - Collidable::GetCenter();
        if (toTarget.Length() > max(sprite->GetWidth(), sprite->GetHeight()))
            velocity = toTarget.Normalized() * m_pStats->GetFinalSpeed();
        return;
    }

    //don't apply its vector until we're near the center 
    int cellSize = context.grid->GetCellSize();
    Vector2 cellWorld = context.grid->GridToWorld(myCell);
    Vector2 cellCenter = { cellWorld.x + cellSize * 0.5f,
                            cellWorld.y + cellSize * 0.5f };

    float dx = Collidable::GetCenter().x - cellCenter.x;
    float dy = Collidable::GetCenter().y - cellCenter.y;
    float threshold = cellSize * 0.35f;

    //smaller = closer to center before allowing vector change
    if ((dx * dx + dy * dy) < (threshold * threshold) || adjustCourseTimer < 0) {
        Vector2 flowDir = context.grid->GetFlowVector(myCell, targetCoord);
        if (flowDir.x != 0.f || flowDir.y != 0.f)
            velocity = flowDir * m_pStats->GetFinalSpeed();
        adjustCourseTimer = 5.0f;
    }

    return;
}
