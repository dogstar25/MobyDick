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
		.particleSizeMin = 8.F,
		.particleSizeMax = 8.F,
		.colorRangeBegin = {0,0,0,255},
		.colorRangeEnd = {255,255,255,255},
		.particleSpawnCountMin = 2,
		.particleSpawnCountMax = 2
	};

	static const ParticleEffect spark = {
		.poolId = "SMOKE1_POOL",
		.originMin = {0,0},
		.originMax = {0,0},
		.forceMin = 1,
		.forceMax = 3,
		.lifetimeMin = 1.2F,
		.lifetimeMax = 4.5F,
		.alphaFade = true,
		.angleMin = 0,
		.angleMax = 360,
		.particleSizeMin = 3.F,
		.particleSizeMax = 20.F,
		.colorRangeBegin = {0,0,0,255},
		.colorRangeEnd = {255,255,255,255},
		.particleSpawnCountMin = 10,
		.particleSpawnCountMax = 20
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

	static const ParticleEffect rain = {
		.poolId = "SMOKE1_POOL",
		.originMin = {0,0},
		.originMax = {1280,720},
		.forceMin = 1,
		.forceMax = 3,
		.lifetimeMin = 1.2F,
		.lifetimeMax = 4.5F,
		.alphaFade = true,
		.angleMin = 45,
		.angleMax = 45,
		.particleSizeMin = 3.F,
		.particleSizeMax = 3.F,
		.colorRangeBegin = {255,255,255,255},
		.colorRangeEnd = {255,255,255,255},
		.particleSpawnCountMin = 10,
		.particleSpawnCountMax = 200
	};

}
