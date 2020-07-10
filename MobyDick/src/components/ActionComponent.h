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
	ActionComponent(Json::Value definitionJSON, GameObject* gameObject);
	~ActionComponent();

	void moveAction(int direction, int strafe)
	{
		m_actionMap[ACTION_MOVE]->perform(m_gameObject, direction, strafe);
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
