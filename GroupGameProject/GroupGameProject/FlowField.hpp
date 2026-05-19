#ifndef FLOWFIELD_HPP
#define FLOWFIELD_HPP

#include "GridCoord.hpp"
#include "Vector2.hpp"
#include <vector>

struct FlowField {
    std::vector<Vector2> vectors;
    std::vector<bool> reached;
    GridCoord target = { -1, -1 };
    bool dirty = true;
    int computedRadius;

    void Reset(int gridWidth, int gridHeight) {
        int total = gridWidth * gridHeight;
        vectors.assign(total, Vector2(0.f, 0.f));
        reached.assign(total, false);
    }

    void ResetRadius(GridCoord target, int radius, int gridWidth, int gridHeight) {
        int minCol = max(0, target.col - radius);
        int maxCol = min(gridWidth - 1, target.col + radius);
        int minRow = max(0, target.row - radius);
        int maxRow = min(gridHeight - 1, target.row + radius);

        for (int row = minRow; row <= maxRow; ++row) {
            for (int col = minCol; col <= maxCol; ++col) {
                int i = (row * gridWidth) + col;
                vectors[i] = Vector2(0.f, 0.f);
                reached[i] = false;
            }
        }
    }

    bool HasVector(int idx) const {
        return idx >= 0 && idx < (int)reached.size() && reached[idx];
    }
};

#endif