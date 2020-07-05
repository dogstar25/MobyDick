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


	/*
	Template function that builds any type of Component that you pass it
	*/
	template <typename componentType>
	inline componentType* buildComponent(Json::Value componentData)
	{

		componentType* component = new componentType(componentData);

		//string test = typeid(gameObject).name();
		return component;
	}

protected:
	std::shared_ptr<GameObject> m_parentGameObject;


};

#endif

