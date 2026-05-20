

/*
#include "Grid.hpp"
#include <queue>
#include <cfloat>

static const int   DIR_COL[8] = { 0,  1,  0, -1,  1,  1, -1, -1 };
static const int   DIR_ROW[8] = { -1,  0,  1,  0, -1,  1, 1,  -1 };
static const float DIR_COST[8] = { 1.f, 1.f, 1.f, 1.f, 1.414f, 1.414f, 1.414f, 1.414f };

#define index(col, row) ((row * gridWidth) + col)

Structure* Grid::GetWallBetween(GridCoord from, GridCoord to) const {
    int dirC = to.col - from.col;
    int dirR = to.row - from.row;

    if (dirC == 0 && dirR == -1) return GetCell(from)->GetWall(WallDirection::NORTH); //moving north
    if (dirC == 0 && dirR == +1) return GetCell(to)->GetWall(WallDirection::NORTH); //moving south
    if (dirC == +1 && dirR == 0) return GetCell(to)->GetWall(WallDirection::WEST);  //moving east
    if (dirC == -1 && dirR == 0) return GetCell(from)->GetWall(WallDirection::WEST);  //moving west

    return nullptr;
}

float Grid::GetEdgeCost(GridCoord from, GridCoord to, int dirIndex) const {
    GridCell* toCell = GetCell(to);
    if (!toCell) return FLT_MAX;

    float cost = DIR_COST[dirIndex];

    if (dirIndex < 4) {//one edge
        Structure* wall = GetWallBetween(from, to);
        if (wall) {
            float wallCost = wall->GetTraversalCost();
            if (wallCost >= FLT_MAX) return FLT_MAX;
            cost += wallCost;
        }
    }
    else {//diagonal, two edges
        GridCoord vertStep = { from.col, to.row };
        GridCoord horzStep = { to.col, from.row };
        GridCoord diagonalStep = { to.col, to.row };

        Structure* w1 = GetWallBetween(to, vertStep);
        Structure* w2 = GetWallBetween(to, horzStep);
        Structure* w3 = GetWallBetween(from, vertStep);
        Structure* w4 = GetWallBetween(from, horzStep);

        for (Structure* w : { w1, w2, w3, w4 }) {
            if (!w) continue;
            float wc = w->GetTraversalCost();
            if (wc >= FLT_MAX) return FLT_MAX;
            cost += wc;
        }
    }

    return cost;
}

//called when wall placed/removed near coord
void Grid::InvalidateFlowFieldsNear(GridCoord changedAt, int radius) {
    for (auto& [coord, field] : flowFields) {
        if (abs(coord.col - changedAt.col) <= radius &&
            abs(coord.row - changedAt.row) <= radius)
            field.dirty = true;
    }
}

void Grid::InvalidateAllFlowFields() {
    for (auto& [coord, field] : flowFields)
        field.dirty = true;
}

//called by each enemy to find direction to target
Vector2 Grid::GetFlowVector(GridCoord from, GridCoord target) {
    if (!IsValidCoord(from) || !IsValidCoord(target))
        return Vector2(0.f, 0.f);

    auto it = flowFields.find(target);
    if (it == flowFields.end()) {
        ComputeFlowField(target, INT_MAX);//on first pass, fill whole grid
        it = flowFields.find(target);
    }
    else if (it->second.dirty) {
        ComputeFlowField(target, 15);
        it = flowFields.find(target);
    }

    int curr1DIndex = index(from.col, from.row);
    if (!it->second.HasVector(curr1DIndex))//if no path to target from "from"
        return Vector2(0.f, 0.f);

    return it->second.vectors[curr1DIndex];
}

void Grid::ComputeFlowField(GridCoord target, int radius) {
    FlowField& field = flowFields[target];
    field.target = target;
    field.dirty = false;
    field.computedRadius = radius;

    if (!IsValidCoord(target)) return;

    if (radius == INT_MAX) {
        field.Reset(gridWidth, gridHeight); //full reset on first pass
    }
    else {
        field.ResetRadius(target, radius, gridWidth, gridHeight); //only reset within radius
    }


    RunDijkstra(target, field, radius);
    SmoothFlowField(field);
    //DebugDumpFlowField(target, 102, 67, 10);
}

void Grid::RunDijkstra(GridCoord target, FlowField& field, int radius) {

    vector<float> cost(gridWidth * gridHeight, FLT_MAX);//cost to go from each cel to target
    using Entry = pair<float, int>; //define type to hold cost and 1D index
    priority_queue<Entry, vector<Entry>, greater<Entry>> pqueue; //pQueue to store entrys. Requires other two parameters to automaticall

    int startIndex = index(target.col, target.row);
    cost[startIndex] = 0.f;
    field.reached[startIndex] = true;
    field.vectors[startIndex] = Vector2(0.f, 0.f);
    pqueue.push({ 0.f, startIndex });

    //DJIKSTRAS!!!!
    while (!pqueue.empty()) {
        Entry current = pqueue.top(); //get element
        pqueue.pop(); //remove
        float curCost = current.first;
        int cur1DIndex = current.second;

        if (curCost > cost[cur1DIndex]) continue; //stale entry

        int curCol = cur1DIndex % gridWidth;
        int curRow = cur1DIndex / gridWidth;

        if (abs(curCol - target.col) > radius ||
            abs(curRow - target.row) > radius) continue;

        for (int d = 0; d < 8; ++d) {//iterate through directions
            int newCol = curCol + DIR_COL[d];
            int newRow = curRow + DIR_ROW[d];

            if (newCol < 0 || newCol >= gridWidth ||
                newRow < 0 || newRow >= gridHeight) continue; //skip if out of bounds

            float edgeCost = GetEdgeCost({ curCol, curRow }, { newCol, newRow }, d);
            if (edgeCost >= FLT_MAX) continue;//skip if direction not possible

            float newCost = curCost + edgeCost;
            int new1DIndex = index(newCol, newRow);

            if (newCost < cost[new1DIndex]) {//if found better cost
                //update
                cost[new1DIndex] = newCost;
                field.reached[new1DIndex] = true;
                field.vectors[new1DIndex] = Vector2(//vector from new to current
                    -(float)DIR_COL[d],
                    -(float)DIR_ROW[d]
                ).Normalized();
                pqueue.push({ newCost, new1DIndex });
            }
        }
    }
}

//all vectors at this point are either straight or perfectly diagonal
//this function smooths vectors to take a less robotic path
void Grid::SmoothFlowField(FlowField& field) {

    fieldVectorCopyScratch.clear();
    fieldVectorCopyScratch = field.vectors;//take snapshot

    //only smooth modified cells
    int radius = field.computedRadius;
    int minCol = max(0, field.target.col - radius);
    int maxCol = min(gridWidth - 1, field.target.col + radius);
    int minRow = max(0, field.target.row - radius);
    int maxRow = min(gridHeight - 1, field.target.row + radius);

    for (int row = minRow; row <= maxRow; ++row) {
        for (int col = minCol; col <= maxCol; ++col) {
            int cur1DIndex = index(col, row);
            if (!field.reached[cur1DIndex]) continue;

            Vector2 myVec = fieldVectorCopyScratch[cur1DIndex];
            int nextCol = col + (int)roundf(myVec.x);
            int nextRow = row + (int)roundf(myVec.y);

            if (nextCol < 0 || nextCol >= gridWidth ||
                nextRow < 0 || nextRow >= gridHeight) continue;

            int next1DIndex = index(nextCol, nextRow);
            if (!field.reached[next1DIndex]) continue;

            //smoothed, combined vector between two cells
            Vector2 blended = (myVec + fieldVectorCopyScratch[next1DIndex]).Normalized();

            //check if the blended direction would cross a wall from this cell.
            //sample one step in the blended direction and test that edge.
            int blendedCol = col + (int)roundf(blended.x);
            int blendedRow = row + (int)roundf(blended.y);

            if (blendedCol < 0 || blendedCol >= gridWidth ||
                blendedRow < 0 || blendedRow >= gridHeight) continue;

            bool wallBlocked = false;
            int dc = blendedCol - col;
            int dr = blendedRow - row;

            if (dc == 0 || dr == 0) {
                wallBlocked = GetWallBetween({ col, row }, { blendedCol, blendedRow }) != nullptr;
            }
            else {
                //diagonal blend — check all 4 walls at the shared corner
                GridCoord from = { col, row };
                GridCoord to = { blendedCol, blendedRow };
                GridCoord vertStep = { col, blendedRow };
                GridCoord horzStep = { blendedCol, row };

                wallBlocked = GetWallBetween(to, vertStep) != nullptr //WEST wall of to
                    || GetWallBetween(to, horzStep) != nullptr //NORTH wall of horzStep
                    || GetWallBetween(from, vertStep) != nullptr //NORTH wall of from
                    || GetWallBetween(from, horzStep) != nullptr; //WEST wall of horzStep
            }

            if (!wallBlocked) {
                field.vectors[cur1DIndex] = blended;
            }
        }
    }
}

//MADE BY AI
void Grid::DebugDumpFlowField(GridCoord target, int centerCol, int centerRow, int radius) {
    auto it = flowFields.find(target);
    if (it == flowFields.end()) return;
    FlowField& field = it->second;

    FILE* f = nullptr;
    fopen_s(&f, "flowfield_dump.txt", "w");
    if (!f) return;

    for (int row = centerRow - radius; row <= centerRow + radius; ++row) {
        for (int col = centerCol - radius; col <= centerCol + radius; ++col) {
            if (!IsValidCoord({ col, row })) continue;
            int flat = index(col, row);

            if (!field.reached[flat]) {
                fprintf(f, "(%d,%d): UNREACHED\n", col, row);
                continue;
            }

            Vector2 v = field.vectors[flat];
            fprintf(f, "(%d,%d): (%.2f, %.2f)", col, row, v.x, v.y);

            // Check for walls on all 4 edges of this cell
            GridCell* cell = GetCell({ col, row });
            if (cell->GetWall(WallDirection::NORTH)) fprintf(f, " [WALL N]");
            if (cell->GetWall(WallDirection::WEST))  fprintf(f, " [WALL W]");
            // South = north of cell below, East = west of cell to the right
            GridCell* south = GetCell({ col, row + 1 });
            GridCell* east = GetCell({ col + 1, row });
            if (south && south->GetWall(WallDirection::NORTH)) fprintf(f, " [WALL S]");
            if (east && east->GetWall(WallDirection::WEST))   fprintf(f, " [WALL E]");

            fprintf(f, "\n");
        }
    }
    fclose(f);
    printf("Flow field dumped around (%d,%d)\n", centerCol, centerRow);
}
*/