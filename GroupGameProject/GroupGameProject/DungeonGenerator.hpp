#pragma once

#include <string>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <random>
#include "Grid.hpp"
#include "Direction.hpp"
#include "GameContext.hpp"
#include "Sprite.hpp"
#include "EnemySpawner.hpp"

using namespace std;

enum class SpawnLimit {
    Unlimited,
    Unique,
    Count//at most n instances
};

struct JigsawConnector {
    GridCoord localCoord; //position relative to top left corner of room
    Direction direction; //which way connector opens to
};


struct RoomTemplate {
    string name;

    vector<vector<vector<char>>> tiles;  //2d array of chars present in each cell
    vector<JigsawConnector> connectors; //list of places we need other rooms to connect to
    int width;
    int height;

    int minDepth;
    int maxDepth;
    int baseWeight;
    bool rotatable;
    SpawnLimit spawnLimit = SpawnLimit::Unlimited;
    int spawnMax; //used only when spawnLimit == Count
    bool required;
};

class Player;
class DungeonGenerator {
    public:
        static constexpr int HALLWAY_THRESHOLD = 5;

        DungeonGenerator(Player* player);
        bool LoadRooms(const string& folderPath);
        void Generate(const string& startRoomName = "");
        void PrintDungeon() const;
        vector<SpawnLocation> GetEnemySpawnLocations();
    private:
        Player* player;
        vector<vector<vector<char>>> dungeon;//grid
        vector<RoomTemplate> rooms; //all rooms
        array<vector<RoomTemplate*>, DIR_COUNT> dirRooms; //rooms with atleast 1 connector facing in DIR
        unordered_map<string, int> spawnCounts; //number of times room spawned
        vector<RoomTemplate*> requiredRooms; //list of rooms required. if not in dungeon, start over
        vector<SpawnLocation> enemySpawnLocations;

        bool ParseRoomFile(const string& filepath, RoomTemplate& out);
        vector<char> ParseTile(const string& token);
        void ApplyToGrid();

        //placement
        void PlaceRoom(const RoomTemplate& room, GridCoord origin, int depth);
        const RoomTemplate* SelectRoom(int availableDepth, int depth, Direction incomingDir);
        void StampRoom(const RoomTemplate& room, GridCoord origin);
        void StampWallBorder(const RoomTemplate& room, GridCoord origin);
        void PlaceHallway(GridCoord fromCoord, Direction dir, int length);

        //helpers
        bool CanPlace(const RoomTemplate& room, GridCoord origin) const;
        RoomTemplate RotateRoom(const RoomTemplate& src, int turns) const; //rotates 90* turns amount of times
        GridCoord AlignConnectors(const RoomTemplate& room, GridCoord connectorWorld, Direction incomingDir) const;
        int GetAvailableDepth(GridCoord worldCoord, Direction dir) const; //counts available space in dir of coord
        void SealConnector(GridCoord worldCoord);
        bool IsValidCoord(GridCoord c) const;
        bool IsEmpty(GridCoord c) const;
        bool HasFloor(GridCoord c) const;
        bool HasWall(GridCoord c) const;
        void AddTile(GridCoord c, char letter);
        void RemoveTile(GridCoord c, char letter);
        void AddDungeonTileFloor(GridCell* cell);


};