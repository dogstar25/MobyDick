#ifndef ACTION_COMPONENT_H
#define ACTION_COMPONENT_H

#include <map>
#include <memory>

#include "../actions/Action.h"


struct ActionComponent
{
	ActionComponent(Json::Value definitionJSON);

	std::map<int, std::shared_ptr<Action>>m_actionMap;

	void moveAction(Entity entity, int direction, int strafe);
	void rotateAction(Entity entity, float angularVelocity);
	void useAction(Entity entity);
};


#endif
