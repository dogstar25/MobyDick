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

	ParticleObject* getParticle(std::string);

	//Accessor Functions
	const std::map <std::string, std::vector<ParticleObject*>>& objectPool(){
		return m_objectPool;
	}

private:
	ObjectPoolManager();
	~ObjectPoolManager();

	std::map <std::string, std::vector<ParticleObject*>> m_objectPool;



};

#endif