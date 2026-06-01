#ifndef SCENE_HPP
#define SCENE_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <vector>

class Renderer;
class Element;
class Sprite;
using namespace std;
class Scene {

public:
	Scene();
	~Scene();

	virtual bool Initialize();  
	virtual void Process(float deltaTime);
	virtual void Draw(Renderer* renderer);
	virtual void AddElement(Element* e);

protected:
    vector<Sprite*> UI;
    vector<Element*> elements;

    // list for items to add/remove next frame
    // add/remove new items to this instead of elements during runtime
    vector<Element*> elementsToAdd;
    vector<Element*> elementsToDelete;
};



#endif