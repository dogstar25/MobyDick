#pragma once
#include <string>
#include <map>
#include <vector>

class ParticleObject;

using namespace::std;

class ObjectPoolManager
{
public:
	static ObjectPoolManager& instance();
	void init();
	void reset(ParticleObject* particle);

	ParticleObject* get(string);
	map <string, vector<ParticleObject*>> objectPool;

private:
	ObjectPoolManager();
	~ObjectPoolManager();


};

