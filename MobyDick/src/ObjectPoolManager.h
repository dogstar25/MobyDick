#ifndef OBJECT_POOL_MANAGER_H
#define OBJECT_POOL_MANAGER_H


#include "texture.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <optional>

#include <SDL2/SDL.h>
#include <json/json.h>

class GameObject;
class Scene;

class ObjectPoolManager
{
public:

	void init(Json::Value definitionJSON, Scene* parentScene);
	void clear();
	ObjectPoolManager();
	~ObjectPoolManager();

	std::optional<std::shared_ptr<GameObject>> getPooledObject(std::string);
	std::shared_ptr<Texture> getPoolObjectTexture(std::string);

	//Accessor Functions
	const std::unordered_map <std::string, std::vector<std::shared_ptr<GameObject>>>& objectPool() {
		return m_objectPool;
	}

private:

	std::unordered_map <std::string, std::vector<std::shared_ptr<GameObject>>> m_objectPool;


};

#endif