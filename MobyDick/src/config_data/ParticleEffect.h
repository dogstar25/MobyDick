#pragma once

#include <string>

#include "../GameConfig.h"

#include <box2d/box2d.h>
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
		.forceMax = 6,
		.lifetimeMin = 0.5F,
		.lifetimeMax = 115.0F,
		.alphaFade = true,
		.angleMin = 0,
		.angleMax = 15,
		.particleSizeMin = 1.28F,
		.particleSizeMax = 25.50F,
		.colorRangeBegin = {0,0,0,255},
		.colorRangeEnd = {255,255,255,255},
		.particleSpawnCountMin = 1,
		.particleSpawnCountMax = 250
	};
}