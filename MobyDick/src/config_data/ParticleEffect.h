#pragma once

#include <string>

#pragma warning(push,0)
#include <box2d/box2d.h>
#pragma warning(pop)

#include <SDL2/SDL.h>


/*
Particle Effect struct is initialized with values if you do not want to override
*/
struct ParticleEffect {
	std::string poolId = "SMOKE1_POOL";
	b2Vec2 originMin;
	b2Vec2 originMax;
	int forceMin = 50;
	int forceMax = forceMin;
	float lifetimeMin = 2.5;
	float lifetimeMax = 2.5;
	bool alphaFade = true;
	float angleMin = 0;
	float angleMax = 360;
	float particleSizeMin = 1;
	float particleSizeMax = 1;
	SDL_Color colorRangeBegin = {255,255,255,255};
	SDL_Color colorRangeEnd = { 255,255,255,255 };
	int particleSpawnCountMin = 25;
	int particleSpawnCountMax = 25;
};

namespace ParticleEffects {

	static const ParticleEffect ricochet = {
		.poolId = "SMOKE1_POOL",
		.originMin = {0,0},
		.originMax = {0,0},
		.forceMin = 1,
		.forceMax = 4,
		.lifetimeMin = 0.5F,
		.lifetimeMax = 6.0F,
		.alphaFade = true,
		.angleMin = 0,
		.angleMax = 180,
		.particleSizeMin = 1.28F,
		.particleSizeMax = 15.50F,
		.colorRangeBegin = {0,0,0,255},
		.colorRangeEnd = {255,255,255,255},
		.particleSpawnCountMin = 1,
		.particleSpawnCountMax = 21
	};
}