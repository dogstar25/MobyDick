#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <json/json.h>

class GameObject;

class Component
{

public:

	Component();
	~Component();

	virtual void update();

protected:

	std::string m_gameObjectId;
	


};

#endif

