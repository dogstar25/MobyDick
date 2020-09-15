#ifndef OBJECT_POOL_MANAGER_H
#define OBJECT_POOL_MANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <optional>

#include <SDL2/SDL.h>

class GameObject;

class ObjectPoolManager
{
public:
	static ObjectPoolManager& instance();
	void init();

	std::optional<std::shared_ptr<GameObject>> getPooledObject(std::string);
	SDL_Texture* getPoolObjectTexture(std::string);

	//Accessor Functions
	const std::unordered_map <std::string, std::vector<std::shared_ptr<GameObject>>>& objectPool() {
		return m_objectPool;
	}

private:
	ObjectPoolManager();
	~ObjectPoolManager();

	std::unordered_map <std::string, std::vector<std::shared_ptr<GameObject>>> m_objectPool;



};

#endif