#include "Explosion.h"
#include "game.h"



Explosion::Explosion()
{
}

Explosion::Explosion(string poolId, float originX, float originY, float forceMin, float forceMax, int particleCount, int angleMin, int angleMax, bool uniform)
{

	this->poolId = poolId;
	this->forceMin = forceMin;
	this->forceMax = forceMax;
	this->particleCount = particleCount;
	this->angleMin = angleMin;
	this->angleMax = angleMax;
	this->uniform = uniform;
	this->originX = originX;
	this->originY = originY;

}


Explosion::~Explosion()
{
}
