#ifndef OBJECT_POOL_MANAGER_H
#define OBJECT_POOL_MANAGER_H

#include <string>
#include <map>
#include <vector>

class ParticleObject;

class ObjectPoolManager
{
public:
	static ObjectPoolManager& instance();
	void init();
	void reset(ParticleObject* particle);

	ParticleObject* get(std::string);
	std::map <std::string, std::vector<ParticleObject*>> objectPool;

private:
	ObjectPoolManager();
	~ObjectPoolManager();


};

#endif