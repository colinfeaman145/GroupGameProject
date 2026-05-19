#include "Particle.hpp"

Particle::Particle(Sprite* s, Vector2 pos, Vector2 initVel, float rotSpeed, float maxA, float a, float alphaDR) {
	sprite = s;
	position = pos;
	velocity = initVel;
	rotation = 0;
	rotationSpeed = rotSpeed;
	age = 0;
	maxAge = maxA;
	alpha = a;
	alphaDecayRate = alphaDR;
}

Particle::~Particle() {
	delete sprite;
}

void Particle::Process(float deltaTime) {
	age += deltaTime;

	velocity.x *= AIR_RESISTANCE;
	velocity.y -= GRAVITY * deltaTime;
	position = position + (velocity * deltaTime);
	rotation += rotationSpeed;
	if (sprite) {
		sprite->Process(deltaTime);
		sprite->SetRotation(rotation);
		sprite->SetPosition((int)position.x, (int)position.y);
	}

	if (alphaDecayRate > 0) {
		alpha -= alphaDecayRate * deltaTime;
		if (alpha < 0)
			age = 1000000;//kill
	}
}

void Particle::Draw(Renderer* renderer) {
	if (sprite) {
		sprite->SetAlpha(alpha);
		sprite->Draw(renderer);
	}
}
