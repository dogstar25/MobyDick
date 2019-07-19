#pragma once
#include "WorldObject.h"

#include <string>
#include <chrono>

class WorldObject;

using namespace std::chrono;

class ParticleObject :
	public WorldObject
{
public:
	ParticleObject();
	ParticleObject(string, int, int, int);
	~ParticleObject();

	void update();
	void render();

	float lifetime, lifetimeRemaining;
	bool isAvailable;
	string poolId;

	steady_clock::time_point time_snapshot;

};

