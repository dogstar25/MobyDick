#ifndef PARTICLE_OBJECT_H
#define PARTICLE_OBJECT_H

#include "WorldObject.h"

#include <string>
#include <chrono>

class WorldObject;

class ParticleObject :
	public WorldObject
{
public:
	ParticleObject();
	ParticleObject(std::string, float, float, float);
	~ParticleObject();

	void update();
	void render();

	std::chrono::duration<float, std::milli> lifetime;
	std::chrono::duration<float, std::milli> lifetimeRemaining;
	bool isLifetimeAlphaFade;
	bool hasInfiniteLifetime;
	bool isAvailable;
	std::string poolId;

	std::chrono::steady_clock::time_point time_snapshot;

};

#endif