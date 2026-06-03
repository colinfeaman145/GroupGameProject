#include "DungeonGenerator.hpp"

RoomTemplate DungeonGenerator::RotateRoom(const RoomTemplate& src, int turns) const {

    RoomTemplate result = src;

    for (int t = 0; t < turns; ++t) {
        int oldW = result.width;
        int oldH = result.height;

        //rotate tiles
        vector<vector<vector<char>>> newTiles(oldW, vector<vector<char>>(oldH));
        for (int r = 0; r < oldH; ++r)
            for (int c = 0; c < oldW; ++c)
                newTiles[c][oldH - 1 - r] = result.tiles[r][c];

        result.tiles = move(newTiles);
        result.width = oldH;
        result.height = oldW;

        //rotate connectors
        for (JigsawConnector& conn : result.connectors) {
            int newCol = oldH - 1 - conn.localCoord.row;
            int newRow = conn.localCoord.col;
            conn.localCoord = { newCol, newRow };
            conn.direction = RotateDir(conn.direction);
        }
    }

    return result;
}

//counts empty space in direction
int DungeonGenerator::GetAvailableDepth(GridCoord worldCoord, Direction dir) const {
    GridCoord step = DirStep(dir);
    GridCoord current = { worldCoord.col + step.col, worldCoord.row + step.row };

    int count = 0;
    while (IsValidCoord(current) && IsEmpty(current)) {
        ++count;
        current.col += step.col;
        current.row += step.row;
    }
    return count;
}

//aligns room's connector with connectorWorld
GridCoord DungeonGenerator::AlignConnectors(const RoomTemplate& room, GridCoord connectorWorld, Direction incomingDir) const {
    Direction needed = Opposite(incomingDir);

    GridCoord localMatch = { 0, 0 };
    for (const auto& c : room.connectors) {
        if (c.direction == needed) {//find first connector that matches direciton
            localMatch = c.localCoord;
            break;
        }
    }
    //printf("[AlignConnectors] Matched local(%d,%d)\n", localMatch.col, localMatch.row);

    return { connectorWorld.col - localMatch.col, connectorWorld.row - localMatch.row };
}

bool DungeonGenerator::CanPlace(const RoomTemplate& room, GridCoord origin) const {
    for (int r = 0; r < room.height; ++r) {
        for (int c = 0; c < room.width; ++c) {
            GridCoord world = { origin.col + c, origin.row + r };
            if (!IsValidCoord(world)) return false;

            bool roomTileHasFloor = HasFloor({ world.col, world.row });

            if (roomTileHasFloor && !IsEmpty(world)) return false;
        }
    }
    return true;
}

void DungeonGenerator::SealConnector(GridCoord worldCoord) {
    if (IsValidCoord(worldCoord))
        AddTile(worldCoord, 'X');
}

bool DungeonGenerator::IsValidCoord(GridCoord c) const {
    return c.col >= 0 && c.col < context.grid->GetGridWidth()
        && c.row >= 0 && c.row < context.grid->GetGridHeight();
}

bool DungeonGenerator::IsEmpty(GridCoord c) const {
    if (!IsValidCoord(c)) return false;
    return dungeon[c.row][c.col].empty();
}

bool DungeonGenerator::HasFloor(GridCoord c) const {
    if (!IsValidCoord(c)) return false;
    for (char ch : dungeon[c.row][c.col])
        if (ch == 'F') return true;
    return false;
}

bool DungeonGenerator::HasWall(GridCoord c) const {
    if (!IsValidCoord(c)) return false;
    for (char ch : dungeon[c.row][c.col])
        if (ch == 'X') return true;
    return false;
}

void DungeonGenerator::AddTile(GridCoord c, char letter) {
    if (!IsValidCoord(c)) return;
    auto& cell = dungeon[c.row][c.col];
    if (find(cell.begin(), cell.end(), letter) == cell.end())
        cell.push_back(letter);
}

void DungeonGenerator::RemoveTile(GridCoord c, char letter) {
    if (!IsValidCoord(c)) return;
    auto& cell = dungeon[c.row][c.col];
    cell.erase(remove(cell.begin(), cell.end(), letter), cell.end());
}

void DungeonGenerator::AddDungeonTileFloor(GridCell* cell) {
    int cellSize = context.grid->GetCellSize();
    SDL_Texture* groundTex = context.txm->LoadTexture(context.renderer, "../../assets/sprites/DungeonTextures/dungeon_floor_with_moss.png");
    Sprite* spr = new Sprite();
    spr->Initialize(groundTex, 750, 750, 0, 0, cellSize, cellSize);
    spr->SetColor({ 220, 255, 220, 255 });
    spr->SetDrawLayer(RenderLayer::GROUND);
    cell->SetSprite(spr);
}


//AI GENERATED
void DungeonGenerator::PrintDungeon() const {
    int rows = (int)dungeon.size();
    int cols = rows > 0 ? (int)dungeon[0].size() : 0;

    // Find bounding box of non-empty tiles
    int minRow = rows, maxRow = -1;
    int minCol = cols, maxCol = -1;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (!dungeon[r][c].empty()) {
                minRow = min(minRow, r);
                maxRow = max(maxRow, r);
                minCol = min(minCol, c);
                maxCol = max(maxCol, c);
            }
        }
    }

    if (maxRow == -1) {
        printf("[DungeonGenerator] Dungeon is empty.\n");
        return;
    }

    printf("[DungeonGenerator] Dungeon (%d-%d, %d-%d):\n", minCol, maxCol, minRow, maxRow);

    for (int r = minRow; r <= maxRow; ++r) {
        for (int c = minCol; c <= maxCol; ++c) {
            const auto& cell = dungeon[r][c];
            if (cell.empty()) {
                printf("  "); // Empty space within bounds
            }
            else if (cell.size() == 1) {
                printf("%c ", cell[0]);
            }
            else {
                // Multiple tiles: print first char, flag with '*'
                printf("%c*", cell[0]);
            }
        }
        printf("\n");
    }
}