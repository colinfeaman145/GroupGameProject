#include "DungeonGenerator.hpp"

/*
Dungeon Generator code that actually places things to dungeon array
*/

void DungeonGenerator::Generate(const string& startRoomName) {

    //reset
    spawnCounts.clear();
    for (auto& row : dungeon)
        for (auto& cell : row)
            cell.clear();

    // Pick starting room
    const RoomTemplate* startRoom = nullptr;

    if (!startRoomName.empty()) {
        for (const auto& room : rooms) {
            if (room.name == startRoomName) {
                startRoom = &room;
                break;
            }
        }
        if (!startRoom)
            printf("DungeonGenerator: start room '%s' not found, picking randomly\n", startRoomName.c_str());
    }

    //pick random start room
    if (!startRoom) {
        vector<RoomTemplate*> pool;
        for (auto& room : rooms)
            if (room.minDepth == 0)
                pool.push_back(&room);
        if (pool.empty()) { printf("DungeonGenerator: no valid start rooms!\n"); return; }
        uniform_int_distribution<int> pick(0, (int)pool.size() - 1);
        startRoom = pool[pick(gen)];
    }

    GridCoord center = {
        context.grid->GetGridWidth() / 2 - startRoom->width / 2,
        context.grid->GetGridHeight() / 2 - startRoom->height / 2
    };

    PlaceRoom(*startRoom, center, 0); //recurse
    ApplyToGrid();
}

void DungeonGenerator::PlaceRoom(const RoomTemplate& room, GridCoord origin, int depth) {

    StampRoom(room, origin); //place in grid

    spawnCounts[room.name]++;

    uniform_int_distribution coinFlip(depth, 7);
    for (const auto& jigsaw : room.connectors) {

        GridCoord connectorWorld = {
            origin.col + jigsaw.localCoord.col,
            origin.row + jigsaw.localCoord.row
        };
        Direction dir = jigsaw.direction;

        GridCoord prevConnector = {
            DirStep(Opposite(dir)).col + connectorWorld.col,
            DirStep(Opposite(dir)).row + connectorWorld.row
        };
        if (!HasFloor(prevConnector) && coinFlip(gen) == 7) {//chance to not expand, increases with further depth(Dont try if already connected)
            SealConnector(connectorWorld);
            continue;
        }

        int available = GetAvailableDepth(connectorWorld, dir);

        if (available < HALLWAY_THRESHOLD) {
            PlaceHallway(connectorWorld, dir, available);
            continue;
        }

        int placementTries = 5;
        while (placementTries > 0) { //try place 5 times beore giving up
            const RoomTemplate* selected = SelectRoom(available, depth + 1, dir);
            if (!selected) {
                placementTries--;
                continue;
            }

            GridCoord newOrigin = AlignConnectors(*selected, connectorWorld, dir);
            if (CanPlace(*selected, newOrigin)) {
                PlaceRoom(*selected, newOrigin, depth + 1);
                break;
            }
            else {
                placementTries--;//repeat if cant place
            }
        }
        if (placementTries == 0) { //couldn't find room that fit
            SealConnector(connectorWorld);
        }
    }
}

const RoomTemplate* DungeonGenerator::SelectRoom(int availableDepth, int depth, Direction incomingDir) {

    Direction needed = Opposite(incomingDir);

    struct Candidate { const RoomTemplate* room; int weight; };
    vector<Candidate> candidates;

    for (const RoomTemplate* room : dirRooms[static_cast<int>(needed)]) {

        //check depth eligibility
        if (depth < room->minDepth) continue;
        if (depth > room->maxDepth) continue;

        //check spawn limit
        if (room->spawnLimit != SpawnLimit::Unlimited) {
            auto it = spawnCounts.find(room->name);
            int placed = (it != spawnCounts.end()) ? it->second : 0;
            if (placed >= room->spawnMax) continue;
        }

        //shallow size check
        int sizeInDir = 0;
        switch (incomingDir) {
        case Direction::N:
        case Direction::S:
            sizeInDir = room->height; break;
        case Direction::E:
        case Direction::W:
            sizeInDir = room->width;  break;
        }
        if (sizeInDir > availableDepth) continue;

        candidates.push_back({ room, room->baseWeight });
    }

    if (candidates.empty()) return nullptr;

    //weighted random selection
    int sum = 0;
    for (const auto& c : candidates)
        sum += c.weight;

    uniform_int_distribution<int> dist(0, sum - 1);
    int roll = dist(gen);
    for (const auto& c : candidates) {
        if (roll < c.weight) return c.room;
        roll -= c.weight;
    }

    return candidates.back().room;
}

//places room in dungeon(grid)
void DungeonGenerator::StampRoom(const RoomTemplate& room, GridCoord origin) {
    //PrintDungeon();
    for (int r = 0; r < room.height; ++r) {
        for (int c = 0; c < room.width; ++c) {
            GridCoord world = { origin.col + c, origin.row + r };
            if (!IsValidCoord(world)) continue;

            auto& cell = dungeon[world.row][world.col];
            const auto& src = room.tiles[r][c];
            if (!src.empty())
                cell = src;
        }
    }
}

void DungeonGenerator::PlaceHallway(GridCoord fromCoord, Direction dir, int length) {
    GridCoord step = DirStep(dir);
    auto [perpA, perpB] = Perpendiculars(dir);
    GridCoord pA = DirStep(perpA);
    GridCoord pB = DirStep(perpB);

    GridCoord current = { fromCoord.col + step.col, fromCoord.row + step.row };

    //printf("[PlaceHallway] from (%d,%d) dir=%d length=%d\n",
        //fromCoord.col, fromCoord.row, (int)dir, length);
    // and inside the loop:

    for (int i = 0; i < length + 1; ++i) {
        //printf("  step(%d,%d) HasFloor=%d HasWall=%d IsEmpty=%d\n",
            //current.col, current.row, HasFloor(current), HasWall(current), IsEmpty(current));
        if (!IsValidCoord(current)) break;

        if (HasFloor(current)) break; //reached another room's floor

        if (HasWall(current)) {
            // Punch through the wall and stop
            RemoveTile(current, 'X');
            AddTile(current, 'F');
            break;
        }

        // Open corridor tile
        AddTile(current, 'F');

        // Side walls (only if tile is empty)
        GridCoord sideA = { current.col + pA.col, current.row + pA.row };
        GridCoord sideB = { current.col + pB.col, current.row + pB.row };
        if (IsValidCoord(sideA) && IsEmpty(sideA)) AddTile(sideA, 'X');
        if (IsValidCoord(sideB) && IsEmpty(sideB)) AddTile(sideB, 'X');

        current.col += step.col;
        current.row += step.row;
    }
}