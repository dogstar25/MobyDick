#pragma once
#include <string>

#include <SDL.h>

using namespace std;
class ParticleEmission
{
public:
	ParticleEmission();
	ParticleEmission(
		string poolId,
		int originX,
		int originY,
		int forceMin,
		int forceMax,
		float lifetimeMin,
		float lifetimeMax,
		bool alphaFade,
		int angleMin,
		int angleMax,
		float particleSizeMin,
		float particleSizeMax,
		SDL_Color colorRangeBegin,
		SDL_Color colorRangeEnd,
		int particleSpawnCountMin,
		int particleSpawnCountMax
		);

	~ParticleEmission();


	string 
		poolId;

	short
		forceMin,
		forceMax,
		particleCountMin,
		particleCountMax,
		lifetimeMin,
		lifetimeMax;

	float
		angleMin,
		angleMax,
		originX,
		originY,
		particleSizeMin,
		particleSizeMax;

	bool
		uniform,
		alphaFade;

	SDL_Color
		colorRangeBegin,
		colorRangeEnd;

};

