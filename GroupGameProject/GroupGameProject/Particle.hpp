#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "Vector2.hpp"

#define GRAVITY -100
#define AIR_RESISTANCE 0.99

class Sprite;
class Renderer;

class Particle
{
public:
	Sprite* sprite;
	Vector2 position;
	Vector2 velocity;
	float rotation;
	float rotationSpeed;
	float age;
	float maxAge;
	float alpha;
	float alphaDecayRate;

	Particle(Sprite* s, Vector2 pos, Vector2 initVel, float rotSpeed, float maxAge, float a, float alphaDecayRate);
	~Particle();
	void Process(float deltaTime);
	void Draw(Renderer* renderer);
};

#endif