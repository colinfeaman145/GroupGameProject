#ifndef AI_HPP
#define AI_HPP

#include "Attackable.hpp"

//An living thing that moves
class AI : public Attackable {

	public:
		void Process(float deltaTime);
		void SetTarget(Collidable* c);
		virtual Collidable* FindNewTarget() = 0;//changes based on AI behavior
		Collidable* GetTarget() const { return target; }
		void Hone();
		virtual void OnStuck() = 0;

		void SetFrozen(float duration);
		bool IsFrozen();

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