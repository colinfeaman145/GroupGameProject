#pragma once
#include "Enemy.hpp"

class FlyingDevil : public Enemy {
public:
	virtual void Initialize(Vector2 pos) override;
};

