#ifndef PARTICLE_EMISSION_H
#define PARTICLE_EMISSION_H

#include <string>

#include <SDL2/SDL.h>
#include <Box2D/Box2D.h>

class ParticleEmission
{
public:
	ParticleEmission();
	ParticleEmission(
		std::string poolId,
		b2Vec2 originMin,
		b2Vec2 originMax,
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

	~ParticleEmission();


	b2Vec2
		originMin,
		originMax;

	std::string
		poolId;

	short
		forceMin,
		forceMax,
		particleCountMin,
		particleCountMax;

	float
		angleMin,
		angleMax,
		//originX,
		//originY,
		lifetimeMin,
		lifetimeMax,
		particleSizeMin,
		particleSizeMax;

	bool
		uniform,
		alphaFade;

	SDL_Color
		colorRangeBegin,
		colorRangeEnd;

};

#endif