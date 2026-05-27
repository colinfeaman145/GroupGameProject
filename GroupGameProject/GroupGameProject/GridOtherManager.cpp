#include "Grid.hpp"

void Grid::AddOther(Entity* e) {
    if (!e) return;

    vector<GridCell*> cells = FindEntityCells(e);
    for (GridCell* cell : cells)
        cell->AddOther(e);
}


void Grid::RemoveOther(Entity* e) {
    if (!e) return;

    vector<GridCell*> cells = FindEntityCells(e);
    for (GridCell* cell : cells)
        cell->RemoveOther(e);
}
