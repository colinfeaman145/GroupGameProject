#ifndef ENUM_TYPES_HPP
#define ENUM_TYPES_HPP

#include "Vector2.hpp"
#include <SDL.h>

//Where enum types or structs go to live after
//they were causing circular dependencies

enum class ShapeType { 
    Square, 
    Circle, 
    Cone, 
    FilledSquare 
};



#endif
