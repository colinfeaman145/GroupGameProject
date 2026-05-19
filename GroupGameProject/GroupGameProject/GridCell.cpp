#include "GridCell.hpp"
#include "Entity.hpp"
#include "Renderer.hpp"
#include <algorithm>

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
    //DrawWalls(renderer);
    //DrawDrops(renderer);
}

void GridCell::Process(float deltaTime, bool isRendered) {
    if (sprite) sprite->Process(deltaTime);
    //ProcessWalls(deltaTime);
    //if (isRendered) ProcessDrops(deltaTime);
}

//void GridCell::DrawWalls(Renderer* renderer) {
//    if (walls[(int)WallDirection::NORTH])  walls[(int)WallDirection::NORTH]->Draw(renderer);
//    if (walls[(int)WallDirection::WEST]) walls[(int)WallDirection::WEST]->Draw(renderer);
//}
//
//void GridCell::ProcessWalls(float deltaTime) {
//    if (walls[(int)WallDirection::NORTH])  walls[(int)WallDirection::NORTH]->Process(deltaTime);
//    if (walls[(int)WallDirection::WEST]) walls[(int)WallDirection::WEST]->Process(deltaTime);
//}

//void GridCell::DrawDrops(Renderer* renderer) {
//    for (Resource* d : drops)
//        d->Draw(renderer);
//}
//
//void GridCell::ProcessDrops(float deltaTime) {
//    for (auto it = drops.begin(); it != drops.end(); ) {//iterate through drops
//        Resource* d = *it;
//        if (d->IsAlive()) {
//            d->Process(deltaTime);
//            ++it;
//        }
//        else {//if set dead, remove it
//            it = drops.erase(it);
//            context.grid->RemoveDrop(d);
//        }
//    }
//    //safe to modify vector
//    for (Resource* d : drops)
//        context.grid->UpdateDropOccupancy(d);
//}


vector<Collidable*> GridCell::GetCollidables() const {
    vector<Collidable*> result;
    result.reserve(enemies.size() + 2 + entities.size());

    for (int i = 0; i < 2; ++i)

    for (Enemy* e : enemies)
        result.push_back(e);

    //for (Resource* d : drops)
    //    result.push_back(d);

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
//assuming wall is free(checked in grid)
//bool GridCell::PlaceWall(WallDirection dir) {
//    int i = (int)dir;
//    if (walls[i]) return false;//if wall there, return
//    walls[i] = true;
//    return true;
//}
//
//bool GridCell::RemoveWall(WallDirection dir) {
//    int i = (int)dir;
//    if (!walls[i]) return false;//if no wall, return
//    delete walls[i];
//    walls[i] = false;
//    return true;
//}
//
//bool GridCell::HasWall(WallDirection dir) const {
//    int i = (int)dir;
//    if (holdingHologramWall[i]) return false; //available to place real wall
//    else return walls[i] != nullptr;
//}


////DROPS
//void GridCell::AddDrop(Resource* drop) {
//    drops.push_back(drop);
//}
//
//void GridCell::RemoveDrop(Resource* drop) {
//    auto it = find(drops.begin(), drops.end(), drop);
//    if (it != drops.end()) {
//        *it = drops.back();
//        drops.pop_back();
//    }
//}


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