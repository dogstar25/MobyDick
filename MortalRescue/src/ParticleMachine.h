#pragma once
#include <vector>
#include <SDL_image.h>

#include "Explosion.h";

using namespace std;

class ParticleMachine
{
public:
	ParticleMachine();
	~ParticleMachine();
	void update();
	
	void runExplosions();
	void execute(Explosion*);
	void add(Explosion*);

	vector <Explosion*> explosions;



	void emit(
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
		int particleSpawnCountMin,
		int particleSpawnCountMax
	);





};

