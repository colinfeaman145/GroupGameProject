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
                        auto isFloor = [&](int c, int r) {
                            for (char t : dungeon[r][c])
                                if (t != 'X') return true;
                            return false;
                            };

                        bool openS = row < context.grid->GetGridHeight() - 1 && isFloor(col, row + 1);

                        string texPath = openS
                            ? "../../assets/sprites/DungeonTextures/dungeonWallBrickNorth.jpg"
                            : "../../assets/sprites/DungeonTextures/dungeonWallBrick.png";

                        SDL_Texture* wallTex = context.txm->LoadTexture(context.renderer, texPath.c_str());
                        Sprite* spr = new Sprite();
                        spr->Initialize(wallTex, 384, 384, 0, 0, 500, 500);
                        spr->SetDrawLayer(RenderLayer::GROUND);

                        cell->PlaceWall(spr);
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
                        door->SetCanCollide(false);
                        cell->AddOther(door);
                        break;
                    }
                    case('J'): {
                        AddDungeonTileFloor(cell);
                        SDL_Texture* jarTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Props/Jar.png");
                        Sprite* spr = new Sprite();
                        spr->Initialize(jarTexture, 51, 49, 0, 0, 500, 500);
                        spr->SetColor({ 220, 255, 220, 255 });
                        spr->SetDrawLayer(RenderLayer::GROUND);
                        Prop* jar = new Prop();
                        jar->Initialize("Jar", cell->GetPosition(), spr);
                        cell->AddOther(jar);
                        break;
                    }
                    case('C'): {
                        AddDungeonTileFloor(cell);
                        SDL_Texture* scullTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Props/Scull.png");
                        Sprite* spr = new Sprite();
                        spr->Initialize(scullTexture, 49, 47, 0, 0, 500, 500);
                        spr->SetColor({ 220, 255, 220, 255 });
                        spr->SetDrawLayer(RenderLayer::GROUND);
                        Prop* scull = new Prop();
                        scull->Initialize("Scull", cell->GetPosition(), spr);
                        cell->AddOther(scull);
                        break;
                    }
                    case('M'): {
                        AddDungeonTileFloor(cell);
                        SDL_Texture* magictableTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Props/Magic_Table.png");
                        Sprite* spr = new Sprite();
                        spr->Initialize(magictableTexture, 67, 45, 0, 0, 500, 500);
                        spr->SetColor({ 220, 255, 220, 255 });
                        spr->SetDrawLayer(RenderLayer::GROUND);
                        Prop* mt = new Prop();
                        mt->Initialize("MagicTable", cell->GetPosition(), spr);
                        cell->AddOther(mt);
                        break;
                    }
                    case('A'): {
                        AddDungeonTileFloor(cell);
                        SDL_Texture* magictable1Texture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Props/Magic_Table_1.png");
                        Sprite* spr = new Sprite();
                        spr->Initialize(magictable1Texture, 55, 52, 0, 0, 500, 500);
                        spr->SetColor({ 220, 255, 220, 255 });
                        spr->SetDrawLayer(RenderLayer::GROUND);
                        Prop* mt1 = new Prop();
                        mt1->Initialize("MagicTableOne", cell->GetPosition(), spr);
                        cell->AddOther(mt1);
                        break;
                    }
                    case('V'): {
                        AddDungeonTileFloor(cell);
                        SDL_Texture* magictable2Texture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Props/Magic_Table_2.png");
                        Sprite* spr = new Sprite();
                        spr->Initialize(magictable2Texture, 51, 49, 0, 0, 500, 500);
                        spr->SetColor({ 220, 255, 220, 255 });
                        spr->SetDrawLayer(RenderLayer::GROUND);
                        Prop* mt2 = new Prop();
                        mt2->Initialize("MagicTableTwo", cell->GetPosition(), spr);
                        cell->AddOther(mt2);
                        break;
                    }
                    case('I'): {
                        AddDungeonTileFloor(cell);
                        SDL_Texture* magictable3Texture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Props/Magic_Table_3.png");
                        Sprite* spr = new Sprite();
                        spr->Initialize(magictable3Texture, 36, 72, 0, 0, 500, 500);
                        spr->SetColor({ 220, 255, 220, 255 });
                        spr->SetDrawLayer(RenderLayer::GROUND);
                        Prop* mt3 = new Prop();
                        mt3->Initialize("MagicTableThree", cell->GetPosition(), spr);
                        cell->AddOther(mt3);
                        break;
                    }
                    case('L'): {
                        AddDungeonTileFloor(cell);
                        SDL_Texture* smalltableTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Props/Small_Table.png");
                        Sprite * spr = new Sprite();
                        spr->Initialize(smalltableTexture, 45, 43, 0, 0, 500, 500);
                        spr->SetColor({ 220, 255, 220, 255 });
                        spr->SetDrawLayer(RenderLayer::GROUND);
                        Prop* sm = new Prop();
                        sm->Initialize("SmallTable", cell->GetPosition(), spr);
                        cell->AddOther(sm);
                        break;
                    }
                    case('R'): {
                        AddDungeonTileFloor(cell);
                        SDL_Texture* tree1Texture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Props/Tree_1.png");
                        Sprite* spr = new Sprite();
                        spr->Initialize(tree1Texture, 55, 52, 0, 0, 500, 500);
                        spr->SetColor({ 220, 255, 220, 255 });
                        spr->SetDrawLayer(RenderLayer::GROUND);
                        Prop* tree = new Prop();
                        tree->Initialize("TreeOne", cell->GetPosition(), spr);
                        cell->AddOther(tree);
                        break;
                    }
                    case('O'): {
                        AddDungeonTileFloor(cell);
                        SDL_Texture* tree2Texture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Props/Tree_2.png");
                        Sprite* spr = new Sprite();
                        spr->Initialize(tree2Texture, 62, 59, 0, 0, 500, 500);
                        spr->SetColor({ 220, 255, 220, 255 });
                        spr->SetDrawLayer(RenderLayer::GROUND);
                        Prop* treetwo = new Prop();
                        treetwo->Initialize("TreeTwo", cell->GetPosition(), spr);
                        cell->AddOther(treetwo);
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