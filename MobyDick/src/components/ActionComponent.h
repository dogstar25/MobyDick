#ifndef ACTION_COMPONENT_H
#define ACTION_COMPONENT_H

#include "Component.h"

#include <map>
#include <json/json.h>

#include "../actions/Action.h"
#include "../Globals.h"

class GameObject;

class ActionComponent : public Component
{
public:
	ActionComponent(std::string gameObjectId, Json::Value componentJSON, GameObject* gameObject);
	~ActionComponent();

	void moveAction()
	{
		m_actionMap[ACTION_MOVE]->perform(m_gameObject);
	}
	void rotateAction()
	{
		m_actionMap[ACTION_ROTATE]->perform(m_gameObject);
	}
	void useAction()
	{
		m_actionMap[ACTION_USE]->perform(m_gameObject);
	}

	std::map<int, std::shared_ptr<Action>>m_actionMap;

};


#endif
