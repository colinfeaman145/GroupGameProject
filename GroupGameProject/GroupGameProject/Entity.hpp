#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Vector2.hpp"
#include "Element.hpp"
#include "Collidable.hpp"

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

		// setter
		virtual void SetPosition(Vector2 pos);
		void SetVelocity(Vector2 vel);
		void SetOccupancy(GridOccupancy occ);

	protected:
		Sprite* sprite;
		float radius;//for collision
		Vector2 position;
		Vector2 velocity;
		GridOccupancy occupancy;//what gridCells the entity is in(used for collision detection)

};

#endif