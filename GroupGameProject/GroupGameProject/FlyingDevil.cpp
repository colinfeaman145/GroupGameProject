#include "FlyingDevil.hpp"

void FlyingDevil::Initialize(Vector2 pos) {
	LoadEntityDataFromJson("FlyingDevil");
	Enemy::Initialize(pos);
}




