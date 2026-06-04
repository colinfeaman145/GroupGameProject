#include <algorithm>
#include "GridCell.hpp"
#include "Entity.hpp"
#include "Renderer.hpp"
#include "Sprite.hpp"
#include "Enemy.hpp"
#include "Grid.hpp"
#include "GameContext.hpp"

GridCell::GridCell(Sprite* spr) {
    coords = { -1, -1 };
    sprite = spr;
    enemies.reserve(16);
    isWall = false;
    SetCollisionBound(CollisionShape::MakeAABB(context.grid->GetCellSize(), context.grid->GetCellSize()));
    SetCanCollide(false);
}

void GridCell::SetSprite(Sprite* spr) {
    sprite = spr;
    spr->SetPosition(position);
}

void GridCell::SetPosition(Vector2 worldPos) {
    if (sprite) sprite->SetPosition((int)worldPos.x, (int)worldPos.y);
    position = worldPos;
}

void GridCell::SetCoords(GridCoord gridPos) {
    coords = gridPos;
}

Vector2 GridCell::GetCenter() const {
    return position + (context.grid->GetCellSize() / 2);
}

void GridCell::Draw(Renderer* renderer) {
    if (sprite) sprite->Draw(renderer);
    for (Enemy* e : enemies) {
        if (e->hasBeenDrawn) continue;
        e->Draw(renderer);
    }
    for (Entity* e : entities) {
        if (e->hasBeenDrawn) continue;
        e->Draw(renderer);
    }

    Color c = CanCollide() ? Color(255, 50, 50, 255) : Color(50, 255, 50, 255);
    Collidable::Draw(renderer, c);
}

void GridCell::Process(float deltaTime, bool isRendered) {
    if (sprite) sprite->Process(deltaTime);
    for (Enemy* e : enemies) {
        if (e->hasBeenProcessed) continue;
        e->Process(deltaTime);
        context.grid->UpdateEnemyOccupancy(e);
    }

    for (Entity* e : entities) {
        if (e->hasBeenProcessed) continue;
        e->Process(deltaTime);
        context.grid->UpdateOccupancy(e, &GridCell::AddOther, &GridCell::RemoveOther);
    }

	std::erase_if(entities, [](Entity* const& p) { return p->isToBeDeleted;});
}

vector<Collidable*> GridCell::GetCollidables() const {
    vector<Collidable*> result;
    result.reserve(enemies.size() + 2 + entities.size());

    for (int i = 0; i < 2; ++i)

    for (Enemy* e : enemies)
        result.push_back(e);

     for (Entity* e : entities)
        result.push_back(e);

    return result;
}


//ENEMIES
void GridCell::AddEnemy(Enemy* enemy) {
    enemies.push_back(enemy);
}

void GridCell::RemoveEnemy(Enemy* enemy) {
    auto it = std::find(enemies.begin(), enemies.end(), enemy);
    if (it != enemies.end()) {
        *it = enemies.back();
        enemies.pop_back();
    }
}

void GridCell::ClearEnemies() {
    enemies.clear();
}

//WALLS
bool GridCell::PlaceWall(Direction dir) {
    SetCanCollide(true);
    isWall = true;
    return isWall;
}

bool GridCell::RemoveWall(Direction dir) {
    SetCanCollide(false);
    isWall = false;
    return isWall;
}

bool GridCell::IsWall() const {
    return isWall;
}


//OTHER
void GridCell::AddOther(Entity* e) {
    entities.push_back(e);
}

void GridCell::RemoveOther(Entity* e) {
    auto it = find(entities.begin(), entities.end(), e);
    if (it != entities.end()) {
        *it = entities.back();
        entities.pop_back();
    }
}

//COLLISION
void GridCell::HandleCollision(Collidable* other, Vector2 penetration) {
    return;
}