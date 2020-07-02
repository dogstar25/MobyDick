#ifndef COMPONENT_COMPONENT_H
#define COMPONENT_COMPONENT_H

#include <memory>

#include "../GameObject.h"

class Component
{

public:

	Component();
	Component(GameObject* parentGameObject);
	~Component();

	virtual void update();
	virtual void render();

protected:
	std::shared_ptr<GameObject> m_parentGameObject;


};

#endif

