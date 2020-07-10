#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <json/json.h>

class GameObject;

class Component
{

public:

	Component() : m_gameObject(NULL) {}
	Component(GameObject * gameObject) : m_gameObject(gameObject) {}
	~Component() {}
	//void setGameObject(GameObject* gameObject) {
	//	m_gameObject = std::make_shared<GameObject>(*gameObject);
	//}
	virtual void update();

	std::string m_gameObjectId;
	//std::shared_ptr<GameObject> m_gameObject;
	GameObject* const m_gameObject;
	


};

#endif

