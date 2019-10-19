#pragma once
#include <vector>
#include <SDL_image.h>

#include "ParticleEmission.h";

using namespace std;

class ParticleMachine
{
public:
	ParticleMachine();
	~ParticleMachine();
	void update();
	
	void runParticleEmissions();
	void add(ParticleEmission*);
	void fireBullet(
		string poolId,
		int originX,
		int originY,
		int angle,
		int force
	);

	vector <ParticleEmission*> particleEmissions;

	void emit(
		string poolId,
		float originX,
		float originY,
		int forceMin,
		int forceMax,
		float lifetimeMin,
		float lifetimeMax,
		bool alphaFade,
		float angleMin,
		float angleMax,
		float particleSizeMin,
		float particleSizeMax,
		SDL_Color colorRangeBegin,
		SDL_Color colorRangeEnd,
		int particleSpawnCountMin,
		int particleSpawnCountMax
	);

};

