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

	void moveAction(GameObject* gameObject, int direction, int strafe);
	void rotateAction(GameObject* gameObject,float angularVelocity);
	void useAction(GameObject* gameObject);

	std::map<int, std::shared_ptr<Action>>m_actionMap;

};


#endif
