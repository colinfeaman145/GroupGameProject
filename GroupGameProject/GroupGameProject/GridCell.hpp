#ifndef GRID_CELL_HPP
#define GRID_CELL_HPP
#include <vector>
#include "Vector2.hpp"
#include "GridCoord.hpp"

class Nature;
class Foliage;
class Sprite;
class Enemy;
class Entity;
class Collidable;
class Renderer;

using namespace std;
class GridCell {
public:
    GridCell(Sprite* spr);
    ~GridCell() = default;

    void SetSprite(Sprite* spr);
    void SetPosition(Vector2 worldPos); //called once when grid is 
    void SetCoords(GridCoord gridPos);
    Sprite* GetSprite() const { return sprite; }
    GridCoord GetCoords() const { return coords; }
    Vector2 GetPosition() const { return position; }
    void Draw(Renderer* renderer);
    void Process(float deltaTime, bool isRendered);
    //void DrawWalls(Renderer* renderer);
    //void ProcessWalls(float deltaTime);
    //void DrawDrops(Renderer* renderer);
    //void ProcessDrops(float deltaTime);

    vector<Collidable*> GetCollidables() const;

    //enemy
    void AddEnemy(Enemy* enemy);
    void RemoveEnemy(Enemy* enemy);
    void ClearEnemies();
    const vector<Enemy*>& GetEnemies() const { return enemies; }

    //walls
    //bool PlaceWall(WallDirection dir);
    //bool RemoveWall(WallDirection dir);
    //bool HasWall(WallDirection dir) const;

    ////drops
    //void AddDrop(Resource* drop);
    //void RemoveDrop(Resource* drop);

    //other entities
    void AddOther(Entity* p);
    void RemoveOther(Entity* p);

private:
    GridCoord coords;
    Vector2 position;
    Sprite* sprite;
    vector <Entity*> entities;//player, attackCone
    vector<Enemy*> enemies;
    //vector<Resource*> drops;
    bool walls[2];
};

#endif