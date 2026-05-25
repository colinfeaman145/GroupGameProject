#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include "Element.hpp"
#include "Vector2.hpp"
#include "Renderer.hpp"
#include <vector>
#include <random>

class Particle;

using namespace std;
class ParticleEmitter : public Element
{
protected:
	vector<Particle*> particles;
	bool isSpawning;
	mt19937 rng;

	//used for constant emisssion
	Vector2 position;
	float emitTimer;
	float emitRate;

	//particle properties
	SDL_Texture* particleImage;
	int particleImageH, particleImageW;
	uniform_real_distribution<float> red;
	uniform_real_distribution<float> green;
	uniform_real_distribution<float> blue;
	uniform_real_distribution<float> size;

	uniform_real_distribution<float> velocityX;
	uniform_real_distribution<float> velocityY;
	uniform_real_distribution<float> rotationSpeed;
	uniform_real_distribution<float> alpha;
	uniform_real_distribution<float> alphaDecay;
	uniform_real_distribution<float> maxAge;

	//TODO add color range

public:
	ParticleEmitter(SDL_Texture* t, Color cmin, Color cmax, float sizeMin, float sizeMax);
	~ParticleEmitter();
	void Initialize();
	void Initialize(Vector2 vMin, Vector2 vMax, float rMin, float rMax, float aMin, float aMax, float adMin, float adMax, float ageMin, float ageMax);
	void Draw(Renderer* renderer) override;
	void Process(float deltaTime) override;
	void SetPosition(Vector2 pos);
	void StartSpawning(float er);
	void StopSpawning();
	void SpawnParticle();
	void Burst(int n);
};

#endif