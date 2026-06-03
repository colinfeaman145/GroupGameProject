#include "DungeonGenerator.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <cassert>

#include "ItemShopSocket.hpp"
#include "Player.hpp"
#include "EnemyId_2.hpp"
#include "Prop.hpp"

namespace fs = filesystem;

DungeonGenerator::DungeonGenerator(Player* player){
    int rows = context.grid->GetGridHeight();
    int cols = context.grid->GetGridWidth();
    this->player = player;
    dungeon.assign(rows, vector<vector<char>>(cols));
}

//load each room, makes rotated variants if allowed
bool DungeonGenerator::LoadRooms(const string& folderPath) {

    //clear vectors
    rooms.clear();
    for (auto& dir : dirRooms) dir.clear();

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.path().extension() != ".txt") continue; 

        RoomTemplate base;
        if (!ParseRoomFile(entry.path().string(), base)) {
            printf("DungeonGenerator: failed to parse %s\n", entry.path().string().c_str());
            continue;
        }
        base.name = entry.path().stem().string();

        int variantCount = base.rotatable ? 4 : 1;
        for (int turns = 0; turns < variantCount; ++turns) {
            RoomTemplate variant = (turns == 0) ? base : RotateRoom(base, turns);
            rooms.push_back(move(variant));
        }
    }

    //organize required rooms
    for (RoomTemplate& room : rooms) {
        if (room.required)
            requiredRooms.push_back(&room);
    }

    //organize rooms by directions they can connect to
    for (RoomTemplate& room : rooms) {//for each room
        for (int d = 0; d < DIR_COUNT; ++d) { //for each direction
            Direction dir = static_cast<Direction>(d);
            for (const auto& c : room.connectors) { //for each connector
                if (c.direction == dir) { //check if connector is in direction
                    dirRooms[d].push_back(&room);
                    break; //only add once per direction per template
                }
            }
        }
    }

    printf("DungeonGenerator: loaded %d rooms from %s\n", rooms.size(), folderPath.c_str());
    return !rooms.empty();
}

//parses 1 files in /dungeonRooms folder
bool DungeonGenerator::ParseRoomFile(const string& filepath, RoomTemplate& out) {

    ifstream file(filepath);
    if (!file.is_open()) return false;

    out = RoomTemplate{};

    string line;

    //read attributes
    getline(file, line); //@minDepth=
    out.minDepth = stoi(line.substr(10));

    getline(file, line); //@maxDepth=
    out.maxDepth = stoi(line.substr(10));
    
    getline(file, line); //@weight=
    out.baseWeight = stoi(line.substr(8)); 
    
    getline(file, line); //@rotatable=
    string val = line.substr(11);
    out.rotatable = (val == "true" && val != "0");

    getline(file, line); //@limit=
    val = line.substr(7);
    if (val == "unlimited") {
        out.spawnLimit = SpawnLimit::Unlimited;
    }
    else if (val == "unique") {
        out.spawnLimit = SpawnLimit::Unique;
        out.spawnMax = 1;
    }
    else {
        int n = stoi(val);
        if (n == 1) {
            out.spawnLimit = SpawnLimit::Unique;
            out.spawnMax = 1;
        }
        else {
            out.spawnLimit = SpawnLimit::Count;
            out.spawnMax = n;
        }
    }

    getline(file, line); //@required=
    val = line.substr(10);
    out.required = (val == "true" && val != "0");

    int row = 0;

    //read tiles
    while (getline(file, line)) {

        if (line.find_first_not_of(" \t\r\n") == string::npos) continue;

        vector<vector<char>> tileRow;
        int col = 0;
        int i = 0;

        while (i < (int)line.size()) {
            if (line[i] == ' ') { ++i; continue; }

            string token;

            if (line[i] == '[') {
                // Multi-letter token: [A,B,C]
                size_t end = line.find(']', i);
                if (end == string::npos) return false; //didnt find ]
                token = line.substr(i, end - i + 1);
                i = (int)end + 1;
            }
            else {
                token = line.substr(i, 1);
                i += 1;
            }

            vector<char> cellContents = ParseTile(token);

            //deal with connector if it has one
            for (char& tile : cellContents) {
                Direction dir;
                bool hasConnector = false;
                switch (tile) {
                    case 'N': dir = Direction::N; hasConnector = true; break;
                    case 'S': dir = Direction::S; hasConnector = true; break;
                    case 'E': dir = Direction::E; hasConnector = true; break;
                    case 'W': dir = Direction::W; hasConnector = true; break;
                }
                if (hasConnector) {
                    out.connectors.push_back({ GridCoord{col, row}, dir });
                    break;
                }
            }

            tileRow.push_back(move(cellContents));
            ++col;
        }

        out.tiles.push_back(move(tileRow));
        ++row;
    }

    if (out.tiles.empty()) return false;

    out.height = (int)out.tiles.size();
    out.width = (int)out.tiles[0].size();
    return true;
}

vector<char> DungeonGenerator::ParseTile(const string& token) {
    vector<char> result;
    if (token.empty()) return result;
    if (token[0] == '.') return result; //blank

    if (token[0] == '[') {
        for (char ch : token)
            if (isalnum(ch)) result.push_back(ch); //push if alphanumeric (ignore , or [ ])
    }
    else {
        result.push_back(token[0]);
    }
    return result;
}

void DungeonGenerator::ApplyToGrid() {

    //if any needed room not here, restart
    for (RoomTemplate* r : requiredRooms) {
        if (spawnCounts[r->name] != r->spawnMax) {
            Generate();
            return;
        }
    }

    for (int row = 0; row < context.grid->GetGridHeight(); ++row) {
        for (int col = 0; col < context.grid->GetGridWidth(); ++col) {
            GridCell* cell = context.grid->GetCell(col, row);
            if (!cell) continue;

            const auto& contents = dungeon[row][col];

            //TODO: decide what to do to each cell depending on each letter
            for (char t : contents) {
                switch (t) {
                    case('X'): {
                        cell->PlaceWall(Direction::N);
                        break;
                    }
                    case('T'): {
                        AddDungeonTileFloor(cell);
                        auto shopSocket = new ItemShopSocket();
                        auto itemToSpawn = context.ir->GetRandomItemOfTier(ItemTier::Common);
                        shopSocket->Initialize(cell->GetCenter() - shopSocket->GetRadius(), itemToSpawn.id);
                        cell->AddOther(shopSocket);//does nothing because other does not get drawn by gridCell
                        break;
                    }
                    case('B'): {
                        AddDungeonTileFloor(cell);
                        Enemy* enemy = Entity::CreateEntityFromJson<EnemyId_2>(context.er->Get(2).data);
                        enemy->Initialize(cell->GetCenter() - enemy->GetRadius());
                        cell->AddEnemy(enemy);
                        break;
                    }
                    case('P'): {
                        AddDungeonTileFloor(cell);
                        player->SetPosition(cell->GetCenter());
                        cell->AddOther(player);
                        break;
                    }
                    case('D'): {
                        AddDungeonTileFloor(cell);
						SDL_Texture* groundTex = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Props/doors_leaf_closed.png");
						Sprite* spr = new Sprite();
						spr->Initialize(groundTex, 32, 32, 0, 0, 500, 500);
						spr->SetColor({ 220, 255, 220, 255 });
						spr->SetDrawLayer(RenderLayer::GROUND);
                        Prop* door = new Prop();
                        door->Initialize("Door", cell->GetPosition(), spr);
                        cell->AddOther(door);
                        break;
                    }
                    case('N'):
                    case('E'):
                    case('S'):
                    case('W'):
                    case('F'): {
                        AddDungeonTileFloor(cell);
                        break;
                    }
                    default:
                        break;
                }
            }

        }
    }
}