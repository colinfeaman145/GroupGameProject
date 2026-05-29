#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Vector2.hpp"
#include "Element.hpp"
#include "Collidable.hpp"

#include "json.hpp"

using json = nlohmann::json;

enum Visibility {
	VISIBLE, // is visible and can collide
	HIDDEN,// is invisible but still can collide
	ABSENT // is invisible and cannot collide
};

struct GridOccupancy {
	int minCol, maxCol, minRow, maxRow;

	bool operator==(const GridOccupancy& o) const {
		return minCol == o.minCol && maxCol == o.maxCol
			&& minRow == o.minRow && maxRow == o.maxRow;
	}
};

class Renderer;
class Sprite;

//A thing with a position
class Entity : public virtual Element, public Collidable {

public:
	Entity();
	~Entity();

	// lifecycle functions
	virtual bool Initialize(Vector2 pos, Sprite* spr = nullptr);
	virtual void Draw(Renderer* renderer) override;
	virtual void Process(float deltaTime) override;


	void Rotate(float direction);

	// getter
	Vector2 GetPosition() const override; //returns center
	Vector2 GetCorner();
	Vector2& GetVelocity();
	float GetRadius();
	Vector2 GetFacingDirection();
	Sprite* GetSprite();
	GridOccupancy GetOccupancy() const;
	Visibility IsVisible();

	// setter
	virtual void SetPosition(Vector2 pos);
	void SetVelocity(Vector2 vel);
	void SetOccupancy(GridOccupancy occ);
	void SetVisibliliy(Visibility visible);

	template<typename T>
	static T* CreateEntityFromJson(json data) {
		auto newItem = new T();
		if (auto entity = dynamic_cast<Entity*>(newItem)) {
			entity->data = data;
		}
		else {
			delete newItem;
			throw std::runtime_error("Type T must be derived from Enemy");
		}
		return newItem;
	}

protected:
	Sprite* sprite;
	Visibility visibility;
	float radius;//for collision
	Vector2 position;
	Vector2 velocity;
	GridOccupancy occupancy;//what gridCells the entity is in(used for collision detection)
	json data;

};

#endif