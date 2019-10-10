#pragma once
#include <string>

using namespace std;
class Explosion
{
public:
	Explosion();
	Explosion(string, float, float, float, float, int, int, int, bool);
	~Explosion();


	string 
		poolId;

	short 
		forceMin,
		forceMax,
		particleCount;

	float
		angleMin,
		angleMax,
		originX,
		originY;

	bool
		uniform;




};

