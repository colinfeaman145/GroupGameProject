#include "Grid.hpp"

void Grid::AddEnemy(Enemy* enemy) {
    if (!enemy) return;

    vector<GridCell*> cells = FindEntityCells(enemy);
    for (GridCell* cell : cells)
        cell->AddEnemy(enemy);
}


void Grid::RemoveEnemy(Enemy* enemy) {
    if (!enemy) return;

    vector<GridCell*> cells = FindEntityCells(enemy);
    for (GridCell* cell : cells)
        cell->RemoveEnemy(enemy);
}

void Grid::UpdateEnemyOccupancy(Enemy* enemy) {
    UpdateOccupancy(enemy, &GridCell::AddEnemy, &GridCell::RemoveEnemy);
}