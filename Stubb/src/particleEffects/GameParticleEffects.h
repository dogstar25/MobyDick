#pragma once
#include "particleEffects\BaseParticleEffects.h"

namespace ParticleEffects {

	static const ParticleEffect whaleBubble = {
		.poolId = "BUBBLE1_POOL",
		.originMin = {0,0},
		.originMax = {0,0},
		.forceMin = 0,
		.forceMax = 4,
		.lifetimeMin = 0.5F,
		.lifetimeMax = 5.2F,
		.alphaFade = false,
		.angleMin = 128,
		.angleMax = 140,
		.particleSizeMin = 1.F,
		.particleSizeMax = 12.F,
		.colorRangeBegin = {5,255,255,25},
		.colorRangeEnd = {255,255,255,25},
		.particleSpawnCountMin = 2,
		.particleSpawnCountMax = 15
	};

	static const ParticleEffect shipSmoke = {
		.poolId = "SMOKE1_POOL",
		.originMin = {0,0},
		.originMax = {0,0},
		.forceMin = 2,
		.forceMax = 8,
		.lifetimeMin = 0.5F,
		.lifetimeMax = 3.0F,
		.alphaFade = true,
		.angleMin = 128,
		.angleMax = 145,
		.particleSizeMin = 3.F,
		.particleSizeMax = 7.F,
		.colorRangeBegin = {20,20,20,255},
		.colorRangeEnd = {255,25,25,255},
		.particleSpawnCountMin = 1,
		.particleSpawnCountMax = 8
	};
}
