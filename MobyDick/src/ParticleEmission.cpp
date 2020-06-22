#include "ParticleEmission.h"



ParticleEmission::ParticleEmission()
{
}

ParticleEmission::ParticleEmission(
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
	int particleCountMin,
	int particleCountMax
)
{
	this->poolId = poolId;
	this->originMin = originMin;
	this->originMax = originMax;
	this->forceMin = forceMin;
	this->forceMax = forceMax;
	this->lifetimeMin = lifetimeMin;
	this->lifetimeMax = lifetimeMax;
	this->alphaFade = alphaFade;
	this->angleMin = angleMin;
	this->angleMax = angleMax;
	this->particleSizeMin = particleSizeMin;
	this->particleSizeMax = particleSizeMax;
	this->colorRangeBegin = colorRangeBegin;
	this->colorRangeEnd = colorRangeEnd;
	this->particleCountMin = particleCountMin;
	this->particleCountMax = particleCountMax;


}


ParticleEmission::~ParticleEmission()
{
}
