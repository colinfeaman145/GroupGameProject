#include <algorithm>
#include "GridCell.hpp"
#include "Entity.hpp"
#include "Renderer.hpp"
#include "Sprite.hpp"
#include "Enemy.hpp"

GridCell::GridCell(Sprite* spr) {
    coords = { -1, -1 };
    sprite = spr;
    enemies.reserve(16);
    for (int i = 0; i < 2; ++i) {
        walls[i] = false;
    }
}

void GridCell::SetSprite(Sprite* spr) {
    sprite = spr;
}

void GridCell::SetPosition(Vector2 worldPos) {
    if (sprite) sprite->SetPosition((int)worldPos.x, (int)worldPos.y);
    position = worldPos;
}

void GridCell::SetCoords(GridCoord gridPos) {
    coords = gridPos;
}

void GridCell::Draw(Renderer* renderer) {
    if (sprite) sprite->Draw(renderer);
}

void GridCell::Process(float deltaTime, bool isRendered) {
    if (sprite) sprite->Process(deltaTime);
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