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
	ActionComponent(Json::Value definitionJSON);
	~ActionComponent();

	void moveAction(int direction, int strafe);
	void rotateAction(float angularVelocity);
	void useAction();

	std::map<int, std::shared_ptr<Action>>m_actionMap;

};


#endif
