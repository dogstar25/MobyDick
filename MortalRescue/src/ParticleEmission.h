#pragma once
#include <string>

#include <SDL.h>
#include <Box2D/Box2D.h>

using namespace std;
class ParticleEmission
{
public:
	ParticleEmission();
	ParticleEmission(
		string poolId,
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

	string 
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

