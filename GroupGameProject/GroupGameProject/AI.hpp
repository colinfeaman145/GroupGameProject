#ifndef AI_HPP
#define AI_HPP

#include "Attackable.hpp"

//An living thing that moves
class AI : public Attackable {

	public:
		AI();
		virtual bool Initialize(Vector2 pos, Sprite* spr) override;
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

		//status
		Vector2 previousPosition;
		float stuckTime;
		bool isChasing;
};

#endif