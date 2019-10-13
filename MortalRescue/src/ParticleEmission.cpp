#include "ParticleEmission.h"
#include "game.h"



ParticleEmission::ParticleEmission()
{
}

ParticleEmission::ParticleEmission(
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
	int particleCountMin,
	int particleCountMax
)
{
	this->poolId = poolId;
	this->originX = originX;
	this->originY = originY;
	this->forceMin = forceMin;
	this->forceMax = forceMax;
	this->lifetimeMin = forceMax;
	this->lifetimeMax = forceMax;
	this->alphaFade = alphaFade;
	this->angleMax = angleMax;
	this->particleSizeMin = particleSizeMin;
	this->particleSizeMax = particleSizeMax;
	this->colorRangeBegin = colorRangeBegin;
	this->lifetimeMax = forceMax;
	this->colorRangeEnd = colorRangeEnd;
	this->particleCountMin = particleCountMin;
	this->particleCountMax = particleCountMax;


}


ParticleEmission::~ParticleEmission()
{
}
