#pragma once
#include "particleEffects\BaseParticleEffects.h"

namespace ParticleEffects {

	static const ParticleEffect ricochet = {
		.poolId = "SMOKE1_POOL",
		.originMin = {0,0},
		.originMax = {0,0},
		.forceMin = 5,
		.forceMax = 15,
		.lifetimeMin = 0.1F,
		.lifetimeMax = 2.2F,
		.alphaFade = false,
		.angleMin = 0,
		.angleMax = 360,
		.particleSizeMin = 1.F,
		.particleSizeMax = 10.F,
		.colorRangeBegin = {0,0,0,255},
		.colorRangeEnd = {255,255,255,255},
		.particleSpawnCountMin = 2,
		.particleSpawnCountMax = 25
	};

	static const ParticleEffect deflect = {
		.poolId = "DEFLECT1_POOL",
		.originMin = {0,0},
		.originMax = {0,0},
		.forceMin = 1,
		.forceMax = 6,
		.lifetimeMin = 0.5F,
		.lifetimeMax = 0.5F,
		.alphaFade = true,
		.angleMin = 0,
		.angleMax = 360,
		.particleSizeMin = 32.0F,
		.particleSizeMax = 64.0F,
		//.colorRangeBegin = {0,0,0,255},
		//.colorRangeEnd = {255,255,255,255},
		.particleSpawnCountMin = 1,
		.particleSpawnCountMax = 2
	};

	static const ParticleEffect scrap = {
		.poolId = "SCRAP1_POOL",
		.originMin = {0,0},
		.originMax = {0,0},
		.forceMin = 2,
		.forceMax = 15,
		.lifetimeMin = 0.0F,
		.lifetimeMax = 0.0F,
		.alphaFade = false,
		.angleMin = 0,
		.angleMax = 360,
		.particleSizeMin = 4.F,
		.particleSizeMax = 4.F,
		.colorRangeBegin = {0,0,0,255},
		.colorRangeEnd = {255,255,255,255},
		.particleSpawnCountMin = 2,
		.particleSpawnCountMax = 2
	};

	static const ParticleEffect spark = {
		.poolId = "SMOKE1_POOL",
		.originMin = {0,0},
		.originMax = {0,0},
		.forceMin = 10,
		.forceMax = 15,
		.lifetimeMin = 0.2F,
		.lifetimeMax = 1.5F,
		.alphaFade = true,
		.angleMin = 1,
		.angleMax = 255,
		.particleSizeMin = 3.F,
		.particleSizeMax = 5.F,
		.colorRangeBegin = {255,0,0,255},
		.colorRangeEnd = {255,0,0,255},
		.particleSpawnCountMin = 2,
		.particleSpawnCountMax = 5
	};

	static const ParticleEffect impactSmoke = {
		.poolId = "SMOKE1_POOL",
		.originMin = {0,0},
		.originMax = {0,0},
		.forceMin = 5,
		.forceMax = 10,
		.lifetimeMin = 0.1F,
		.lifetimeMax = 2.2F,
		.alphaFade = true,
		.angleMin = 0,
		.angleMax = 360,
		.particleSizeMin = 1.F,
		.particleSizeMax = 10.F,
		.colorRangeBegin = {255,255,255,255},
		.colorRangeEnd = {255,255,255,255},
		.particleSpawnCountMin = 2,
		.particleSpawnCountMax = 5
	};

}
