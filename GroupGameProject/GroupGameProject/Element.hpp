#ifndef ELEMENT_HPP
#define ELEMENT_HPP

class Renderer;

//VIRTUAL CLASS
class Element {
public:
	bool isToBeDeleted;
	virtual void Draw(Renderer* renderer) = 0;
	virtual void Process(float deltaTime) = 0;
};

#endif