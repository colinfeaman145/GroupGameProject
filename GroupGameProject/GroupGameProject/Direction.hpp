#ifndef DIRECTION_HPP
#define DIRECTION_HPP

#include "GridCoord.hpp"

enum class Direction { 
    N = 0, 
    E = 1, 
    S = 2, 
    W = 3 
};
static constexpr int DIR_COUNT = 4;

inline Direction Opposite(Direction d) {
    switch (d) {
    case Direction::N: return Direction::S;
    case Direction::S: return Direction::N;
    case Direction::E: return Direction::W;
    case Direction::W: return Direction::E;
    }
    return Direction::N;
}

//rotates clockwise once
inline Direction RotateDir(Direction d) {
    switch (d) {
    case Direction::N: return Direction::E;
    case Direction::E: return Direction::S;
    case Direction::S: return Direction::W;
    case Direction::W: return Direction::N;
    }
    return Direction::N;
}

inline GridCoord DirStep(Direction d) {
    switch (d) {
    case Direction::N: return { 0, -1 };
    case Direction::S: return { 0,  1 };
    case Direction::E: return { 1,  0 };
    case Direction::W: return { -1,  0 };
    }
    return { 0, 0 };
}

//returns the two directions perpendicular to d
inline std::pair<Direction, Direction> Perpendiculars(Direction d) {
    if (d == Direction::N || d == Direction::S)
        return { Direction::E, Direction::W };
    return { Direction::N, Direction::S };
}

#endif