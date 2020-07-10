#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <json/json.h>

#include "../Globals.h"

class GameObject;

class Component
{

public:

	Component() : m_gameObject(NULL) {}
	Component(GameObject* gameObject) : m_gameObject(gameObject) {}
	~Component() {}
	/*void setGameObject(std::shared_ptr<GameObject> gameObject) {
		m_gameObject = gameObject;
	}*/
	virtual void update();

	std::string m_gameObjectId;
	GameObject* m_gameObject;

};

#endif

