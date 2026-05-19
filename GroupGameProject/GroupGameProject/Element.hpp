#ifndef ELEMENT_HPP
#define ELEMENT_HPP
#include "Vector2.hpp"
#include "Renderer.hpp"
#include "Collidable.hpp"

//VIRTUAL CLASS
class Element {
public:
	virtual void Draw(Renderer* renderer) = 0;
	virtual void Process(float deltaTime) = 0;
};

#endif