#ifndef GRIDCOORD_HPP
#define GRIDCOORD_HPP

#include <functional>

struct GridCoord {
    int col;
    int row;

    bool operator==(const GridCoord& other) const {
        return col == other.col && row == other.row;
    }
};

struct GridCoordHash {
    size_t operator()(const GridCoord& c) const {

        return std::hash<int>()(c.col) ^ (std::hash<int>()(c.row) << 16);
    }
};

struct GridCoordEqual {
    bool operator()(const GridCoord& a, const GridCoord& b) const {
        return a.col == b.col && a.row == b.row;
    }
};

enum class WallDirection {
    NORTH,
    WEST
};

#endif