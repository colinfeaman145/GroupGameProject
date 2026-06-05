#ifndef GRID_CELL_HPP
#define GRID_CELL_HPP
#include <vector>
#include "Vector2.hpp"
#include "GridCoord.hpp"
#include "Direction.hpp"
#include "Collidable.hpp"
#include "Explosion.hpp"

class Sprite;
class Enemy;
class Entity;

using namespace std;
class GridCell : public Collidable{
public:
    GridCell(Sprite* spr = nullptr);
    ~GridCell() = default;

    void SetSprite(Sprite* spr);
    void SetPosition(Vector2 worldPos); //called once when grid is 
    void SetCoords(GridCoord gridPos);
    Sprite* GetSprite() const { return sprite; }
    GridCoord GetCoords() const { return coords; }
    Vector2 GetPosition() const { return position; }
    Vector2 GetCenter() const;
    void Draw(Renderer* renderer);
    void Process(float deltaTime, bool isRendered);

    virtual void HandleCollision(Collidable* other, Vector2 penetration) override;

    vector<Collidable*> GetCollidables() const;

    //enemy
    void AddEnemy(Enemy* enemy);
    void RemoveEnemy(Enemy* enemy);
    void ClearEnemies();
    void ClearEntities() { entities.clear(); }
    const vector<Enemy*>& GetEnemies() const { return enemies; }

    //walls
    bool PlaceWall(Direction dir);
    bool RemoveWall(Direction dir);
    bool IsWall() const;

    //other entities
    void AddOther(Entity* p);
    void RemoveOther(Entity* p);

private:
    GridCoord coords;
    Vector2 position;
    Sprite* sprite;
    vector <Entity*> entities;//player, attackCone
    vector<Enemy*> enemies;
    vector<Explosion*> explosions;
    //vector<Resource*> drops;
    bool isWall;
};

#endif