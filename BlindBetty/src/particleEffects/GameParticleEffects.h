#pragma once
#include "particleEffects\BaseParticleEffects.h"

namespace ParticleEffects {


	static const ParticleEffect spark = {
		.poolId = "SMOKE1_POOL",
		.originMin = {0,0},
		.originMax = {0,0},
		.forceMin = 1,
		.forceMax = 5,
		.lifetimeMin = 0.2F,
		.lifetimeMax = 1.5F,
		.alphaFade = true,
		.angleMin = 90,
		.angleMax = 90,
		.particleSizeMin = 1.F,
		.particleSizeMax = 3.F,
		.colorRangeBegin = {255,0,0,255},
		.colorRangeEnd = {255,0,0,255},
		.particleSpawnCountMin = 2,
		.particleSpawnCountMax = 70
	};

}
