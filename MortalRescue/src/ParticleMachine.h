#pragma once
#include <vector>

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

};

