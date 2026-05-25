#include "Grid.hpp"

#include <algorithm>
#include <cmath>
#include <unordered_set>

#include "Renderer.hpp"
#include "GameContext.hpp"
#include "Sprite.hpp"
#include "Camera.hpp"
#include "Collision.hpp"

Grid::Grid(int worldWidth, int worldHeight, int cellSize)
    : cellSize(cellSize)
    , gridWidth((int)ceil((float)worldWidth / cellSize))
    , gridHeight((int)ceil((float)worldHeight / cellSize))
    , cells(nullptr) {
}

Grid::~Grid() {
    if (!cells) return;

    for (int row = 0; row < gridHeight; ++row) {
        if (cells[row]) {
            for (int col = 0; col < gridWidth; ++col) {
                delete cells[row][col];
                cells[row][col] = nullptr;
            }
            delete[] cells[row];
            cells[row] = nullptr;
        }
    }
    delete[] cells;
    cells = nullptr;
}

//TODO accept file path and construct grid accordingly
bool Grid::Initialize(SDL_Texture* cellTexture) {

    context.grid = this;
    itemPickupRadius = 1;

    cells = new GridCell * *[gridHeight];
    uniform_int_distribution<int> startNatureGen(1, 10);

    printf("MAKING WORLD GRID\n");
    for (int row = 0; row < gridHeight; ++row) {
        cells[row] = new GridCell * [gridWidth];
        for (int col = 0; col < gridWidth; ++col) {
            Sprite* spr = new Sprite();
            spr->Initialize(cellTexture, 750, 750, 0, 0, cellSize, cellSize);
            spr->SetColor({ 220, 255, 220, 255 });

            GridCell* cell = new GridCell(spr);
            cell->SetCoords(GridCoord(col, row));
            cell->SetPosition(GridToWorld({ col, row }));
            cell->GetSprite()->SetDrawLayer(RenderLayer::GROUND);
            cells[row][col] = cell;
        }
    }

    return true;
}

void Grid::Draw(Renderer* renderer) {
    SDL_Rect vp = renderer->cam->GetViewportWorldRect();

    //draw only nearby cells
    int minCol = max(0, (int)floor((float)vp.x / cellSize) - RENDER_DISTANCE);
    int maxCol = min(gridWidth - 1, (int)ceil((float)(vp.x + vp.w) / cellSize) + RENDER_DISTANCE);
    int minRow = max(0, (int)floor((float)vp.y / cellSize) - RENDER_DISTANCE);
    int maxRow = min(gridHeight - 1, (int)ceil((float)(vp.y + vp.h) / cellSize) + RENDER_DISTANCE);

    for (int row = minRow; row <= maxRow; ++row)
        for (int col = minCol; col <= maxCol; ++col)
            cells[row][col]->Draw(renderer);

}

void Grid::Process(float deltaTime) {

    SDL_Rect vp = context.renderer->cam->GetViewportWorldRect();
    uniform_int_distribution<int> startNatureGen(1, 80000000);//happens more often than youd think(every ~20 seconds)

    int minCol = max(0, (int)floor((float)vp.x / cellSize) - RENDER_DISTANCE);
    int maxCol = min(gridWidth - 1, (int)ceil((float)(vp.x + vp.w) / cellSize) + RENDER_DISTANCE);
    int minRow = max(0, (int)floor((float)vp.y / cellSize) - RENDER_DISTANCE);
    int maxRow = min(gridHeight - 1, (int)ceil((float)(vp.y + vp.h) / cellSize) + RENDER_DISTANCE);

    for (int row = 0; row < gridHeight; ++row) {
        for (int col = 0; col < gridWidth; ++col) {
            bool visible = row >= minRow && row <= maxRow && col >= minCol && col <= maxCol;
            cells[row][col]->Process(deltaTime, visible);
        }
    }
}


bool Grid::IsValidCoord(GridCoord coord) const {
    return coord.col >= 0 && coord.col < gridWidth && coord.row >= 0 && coord.row < gridHeight;
}

GridCell* Grid::GetCell(GridCoord coord) const {
    if (!IsValidCoord(coord)) return nullptr;
    return cells[coord.row][coord.col];
}

GridCell* Grid::GetCell(int col, int row) const {
    return GetCell({ col, row });
}


//CORDINATE COVERSION
GridCoord Grid::WorldToGrid(Vector2 worldPos) const {
    return {
        (int)floor(worldPos.x / cellSize),
        (int)floor(worldPos.y / cellSize)
    };
}

Vector2 Grid::GridToWorld(GridCoord coord) const {
    return Vector2(
        (float)(coord.col * cellSize),
        (float)(coord.row * cellSize)
    );
}

Vector2 Grid::SnapToGrid(Vector2 worldPos) const {
    GridCoord coord = WorldToGrid(worldPos);

    coord.col = max(0, min(coord.col, gridWidth - 1));
    coord.row = max(0, min(coord.row, gridHeight - 1));

    return GridToWorld(coord);
}

//returns cells the occupancy of entity is in
vector<GridCell*> Grid::FindEntityCells(Entity* entity) {
    vector<GridCell*> v;
    if (!entity) return v;

    const GridOccupancy& occ = entity->GetOccupancy();
    for (int row = occ.minRow; row <= occ.maxRow; ++row) {
        for (int col = occ.minCol; col <= occ.maxCol; ++col) {
            if (GridCell* cell = GetCell(col, row))
                v.push_back(cell);
        }
    }
    return v;
}


//NEIGHBOR CHECKING
//returns cells in a radius of cell at input coordinates
vector<GridCell*>& Grid::GetNeighbourCells(GridCoord coord, int radius) {
    neighborCellsScratch.clear();

    for (int row = coord.row - radius; row <= coord.row + radius; ++row)
        for (int col = coord.col - radius; col <= coord.col + radius; ++col)
            if (GridCell* cell = GetCell(col, row))
                neighborCellsScratch.push_back(cell);

    return neighborCellsScratch;
}

//include 1 instance of all collidables in reachable cells
vector<Collidable*>& Grid::GetNearbyCollidables(GridCoord coord, int radius) {

    collidableScratch.clear();
    collidableSeen.clear();

    for (GridCell* cell : GetNeighbourCells(coord, radius)) {
        for (Collidable* c : cell->GetCollidables()) {
            if (collidableSeen.insert(c).second) //returns false if alrady present
                collidableScratch.push_back(c);
        }
    }
    return collidableScratch;
}

//check if entity collides with anything. Act if they do
bool Grid::ResolveCollisions(Entity* entity) {

    if (!entity) return false;
    bool anyCollision = false;

    //find search area
    const GridOccupancy& occ = entity->GetOccupancy();
    int searchRadius = max(occ.maxCol - occ.minCol, occ.maxRow - occ.minRow) + 3;
    GridCoord center = { (occ.minCol + occ.maxCol) / 2, (occ.minRow + occ.maxRow) / 2 };

    //get entity to check for collision
    vector<Collidable*> candidates = GetNearbyCollidables(center, searchRadius);

    //check entities for collision
    for (Collidable* other : candidates) {
        if (!other) continue;//if destroyed
        if (other == entity) continue;//if self

        Vector2 penetration;
        if (Collision::TestShapes(
            entity->GetCollisionBound(), entity->GetPosition(),
            other->GetCollisionBound(), other->GetPosition(),
            penetration))
        {
            entity->HandleCollision(other, penetration);
            other->HandleCollision(entity, -penetration);
            anyCollision = true;
        }
    }
    return anyCollision;
}

bool Grid::HasCollision(Entity* entity) {
    if (!entity) return false;

    const GridOccupancy& occ = entity->GetOccupancy();
    int searchRadius = max(occ.maxCol - occ.minCol, occ.maxRow - occ.minRow) + 3;
    GridCoord center = { (occ.minCol + occ.maxCol) / 2, (occ.minRow + occ.maxRow) / 2 };

    for (Collidable* other : GetNearbyCollidables(center, searchRadius)) {
        if (!other || other == entity) continue;

        Vector2 penetration;
        if (Collision::TestShapes(
            entity->GetCollisionBound(), entity->GetPosition(),
            other->GetCollisionBound(), other->GetPosition(),
            penetration))
        {
            return true;
        }
    }
    return false;
}