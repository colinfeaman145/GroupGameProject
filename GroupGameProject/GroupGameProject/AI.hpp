#ifndef AI_HPP
#define AI_HPP

#include "Attackable.hpp"

//An living thing that moves
class AI : public Attackable {

	public:
		virtual void Process(float deltaTime) override;
		virtual void Draw(Renderer* renderer) override;

		void SetTarget(Collidable* c);
		Collidable* GetTarget() const { return target; }
		void Hone();
		virtual void OnStuck() = 0;

	protected:
		Collidable* target;

		//traits
		float adjustCourseTimer;
		float currentRetargetTime;
		float retargetCooldown;
		int framesSinceLastHone;
		int targetRadius;
		float movementSpeed;

		//status
		Vector2 previousPosition;
		float stuckTime;
		float frozenTime;
		bool frozen;
};

#endif