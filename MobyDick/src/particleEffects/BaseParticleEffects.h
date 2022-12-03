#pragma once

#include <string>

//#include "../GameConfig.h"

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

	void setColorRangeBegin(SDL_Color color) {
		colorRangeBegin = { color.r, color.g, color.b, color.a };
	}

	void setColorRangeEnd(SDL_Color color) {
		colorRangeEnd = { color.r, color.g, color.b, color.a };
	}

};

namespace ParticleEffects {

	static const ParticleEffect smoke = {
		.poolId = "SMOKE1_POOL",
		.originMin = {0,0},
		.originMax = {0,0},
		.forceMin = 5,
		.forceMax = 15,
		.lifetimeMin = 2.5F,
		.lifetimeMax = 3.0F,
		.alphaFade = true,
		.angleMin = 0,
		.angleMax = 360,
		.particleSizeMin = 5.F,
		.particleSizeMax = 12.F,
		//.colorRangeBegin = {255,255,0,255},
		//.colorRangeEnd = {255,255,255,255},
		.particleSpawnCountMin = 3,
		.particleSpawnCountMax = 10
	};


}