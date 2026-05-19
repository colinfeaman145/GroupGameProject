#include "ParticleEmitter.hpp"

using namespace std;

ParticleEmitter::ParticleEmitter(SDL_Texture* t, Color cmin, Color cmax, float sizeMin, float sizeMax) {
	rng = mt19937(random_device{}());
	particleImage = t;
	SDL_QueryTexture(particleImage, nullptr, nullptr, &particleImageW, &particleImageH);
	isSpawning = false;

	red = uniform_real_distribution<float>(cmin.r, cmax.r);
	green = uniform_real_distribution<float>(cmin.g, cmax.g);
	blue = uniform_real_distribution<float>(cmin.b, cmax.b);
	size = uniform_real_distribution<float>(sizeMin, sizeMax);
}

ParticleEmitter::~ParticleEmitter() {}

void ParticleEmitter::Initialize() {
	velocityX = uniform_real_distribution<float>(-100, -300);
	velocityY = uniform_real_distribution<float>(100, -100);
	rotationSpeed = uniform_real_distribution<float>(2, 2);
	alpha = uniform_real_distribution<float>(200, 255);
	alphaDecay = uniform_real_distribution<float>(10, 100);
	maxAge = uniform_real_distribution<float>(1, 3);
}

void ParticleEmitter::Initialize(Vector2 vMin, Vector2 vMax, float rMin, float rMax, float aMin, float aMax, float adMin, float adMax, float ageMin, float ageMax) {
	velocityX = uniform_real_distribution<float>(vMin.x, vMax.x);
	velocityY = uniform_real_distribution<float>(vMin.y, vMax.y);
	rotationSpeed = uniform_real_distribution<float>(rMin, rMax);
	alpha = uniform_real_distribution<float>(aMin, aMax);
	alphaDecay = uniform_real_distribution<float>(adMin, adMax);
	maxAge = uniform_real_distribution<float>(ageMin, ageMax);
}

void ParticleEmitter::Process(float deltaTime) {
	if (isSpawning) {
		emitTimer += deltaTime;

		if (emitTimer >= emitRate) {
			emitTimer = 0;
			SpawnParticle();
		}
	}

	for (int i = particles.size() - 1; i >= 0; i--) {
		Particle* p = particles[i];

		p->Process(deltaTime); //update

		if (p->age >= p->maxAge) { //kill if old
			particles.erase(particles.begin() + i);
			delete p;
		}
	}
}

void ParticleEmitter::Draw(Renderer* renderer) {
	for (Particle* p : particles) {
		p->Draw(renderer);
	}
}

void ParticleEmitter::SetPosition(Vector2 pos) {
	position = pos;
}

void ParticleEmitter::StartSpawning(float er) {
	isSpawning = true;
	emitRate = er;
}

void ParticleEmitter::StopSpawning() {
	isSpawning = false;
}

void ParticleEmitter::SpawnParticle() {
	//define particle image
	float s = size(rng);
	float r = red(rng);
	float g = green(rng);
	float b = blue(rng);
	Sprite* sprite = new Sprite();
	sprite->Initialize(particleImage, particleImageW, particleImageH, 0, 0, s, s);
	sprite->SetColor(Color(r, g, b));
	sprite->SetDrawLayer(RenderLayer::PARTICLE);

	//define particle behavior
	Vector2 vel = Vector2(velocityX(rng), velocityY(rng));
	float rotSpeed = rotationSpeed(rng);
	float age = maxAge(rng);
	float a = alpha(rng);
	float decay = alphaDecay(rng);

	Particle* p = new Particle(
		sprite,
		position,
		vel,
		rotSpeed,
		age,
		a,
		decay
	);
	particles.push_back(p);
}

void ParticleEmitter::Burst(int n) {
	for (int i = 0; i < n; i++) {
		SpawnParticle();
	}
}