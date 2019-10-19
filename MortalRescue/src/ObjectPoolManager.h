#pragma once
#include <string>
#include <map>
#include <vector>

class ParticleObject;

using namespace::std;

class ObjectPoolManager
{
public:
	ObjectPoolManager();
	~ObjectPoolManager();
	void init();
	void reset(ParticleObject* particle);

	ParticleObject* get(string);
	map <string, vector<ParticleObject*>> objectPool;

};

