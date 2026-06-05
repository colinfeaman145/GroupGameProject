#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <SDL.h>
#include "Vector2.hpp"
#include "GridCell.hpp"
#include "Entity.hpp"
#include "FlowField.hpp"


class GridCoord;
class GridCoordHash;
class GridCoordEqual;
class Attackable;
class Grid : public Element {
public:
    Grid(int worldWidth, int worldHeight, int cellSize);
    ~Grid();
    bool Initialize(SDL_Texture* cellSprite = nullptr);
    void Draw(Renderer* renderer) override;
    void Process(float deltaTime) override;

    int GetCellSize() const { return cellSize; }
    int GetGridWidth() const { return gridWidth; }
    int GetGridHeight() const { return gridHeight; }

    GridCell* GetCell(GridCoord coord) const;
    GridCell* GetCell(int col, int row) const;
    bool IsValidCoord(GridCoord coord) const;
    GridCoord WorldToGrid(Vector2 worldPos) const;
    Vector2 GridToWorld(GridCoord coord) const; //returns top-left of cell
    Vector2 SnapToGrid(Vector2 worldPos) const; //snaps a world position to cell origin

    vector<GridCell*> FindEntityCells(Entity* entity);
    vector<GridCell*>& GetNeighbourCells(GridCoord coord, int radius);
    vector<Collidable*>& GetNearbyCollidables(GridCoord coord, int radius);
    vector<Collidable*> GetCollidablesInRadius(const CollisionShape& shape, Vector2 pos);

    template<typename T>
    void UpdateOccupancy(T* entity, void (GridCell::* addFunc)(T*), void (GridCell::* removeFunc)(T*));
    bool ResolveCollisions(Entity* entity);
    bool HasCollision(Entity* entity);
    Attackable* GetRandomEnemyInRange(Entity* entity, int searchRadius);

    //pathfinding
    Vector2 GetFlowVector(GridCoord from, GridCoord target);
    void InvalidateFlowFieldsNear(GridCoord changedAt, int radius);
    void InvalidateAllFlowFields();
    void DebugDumpFlowField(GridCoord target, int centerCol, int centerRow, int radius);

    //Enemies
    void AddEnemy(Enemy* enemy);
    void RemoveEnemy(Enemy* enemy);
    void UpdateEnemyOccupancy(Enemy* enemy);
    void ClearAllEntities();

       //Other
    void AddOther(Entity* e);
    void RemoveOther(Entity* e);

private:

    int itemPickupRadius;

    int cellSize;
    int gridWidth; //in cells
    int gridHeight;

    GridCell*** cells;//2d array of pointers

    //pathfinding
    float GetEdgeCost(GridCoord from, GridCoord to, int dirIndex) const;

    void ComputeFlowField(GridCoord target, int radius);
    void RunDijkstra(GridCoord target, FlowField& field, int radius);
    void SmoothFlowField(FlowField& field);

    unordered_map<GridCoord, FlowField, GridCoordHash, GridCoordEqual> flowFields;

    //memory savers
    vector<GridCell*> neighborCellsScratch;
    vector<Collidable*> collidableScratch;
    vector<Vector2> fieldVectorCopyScratch;
    unordered_set<Collidable*> collidableSeen;
};



//OCCUPANCY MANAGEMENT
template<typename T>
//updates Entities value occupancy
//NOTE: if compiler is throwing an error, ignore it.
void Grid::UpdateOccupancy(T* entity, void (GridCell::* addFunc)(T*), void (GridCell::* removeFunc)(T*)) {

    static_assert(std::is_base_of_v<Entity, T>, "T must be a subclass of Entity");
    if (!entity) return;

    Vector2 pos = entity->GetCollisionBound().WorldPosition(entity->GetPosition());
    float r = entity->GetRadius();

    GridOccupancy next = {
        (int)floor((pos.x - r) / cellSize),
        (int)floor((pos.x + r) / cellSize),
        (int)floor((pos.y - r) / cellSize),
        (int)floor((pos.y + r) / cellSize)
    };

    const GridOccupancy& prev = entity->GetOccupancy();
    if (next == prev) return; //if no change, dont update anything

    //Remove from old cells
    for (int row = prev.minRow; row <= prev.maxRow; ++row) {
        for (int col = prev.minCol; col <= prev.maxCol; ++col) {
            if (GridCell* cell = GetCell(col, row))
                (cell->*removeFunc)(entity);
        }
    }

    //Insert into new cells
    for (int row = next.minRow; row <= next.maxRow; ++row) {
        for (int col = next.minCol; col <= next.maxCol; ++col) {
            if (GridCell* cell = GetCell(col, row))
                (cell->*addFunc)(entity);
        }
    }

    entity->SetOccupancy(next);
}

#endif