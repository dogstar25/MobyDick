#ifndef ACTION_COMPONENT_H
#define ACTION_COMPONENT_H

#include "Component.h"

#include <map>
#include <vector>
#include <optional>

#include <json/json.h>

#include "../actions/Action.h"
#include "../actions/MoveAction.h"
#include "../actions/RotateAction.h"
#include "../actions/InteractAction.h"
#include "../actions/UseAction.h"
#include "../actions/NoAction.h"
#include "../Globals.h"


class GameObject;

class ActionComponent : public Component
{
public:
	ActionComponent(Json::Value definitionJSON);
	~ActionComponent();

	void update();

	void performMoveAction(int direction, int strafe);
	void performRotateAction(float angularVelocity);
	void performUseAction();
	void performUsageAction();
	void performInteractAction();
	void performOnHoverAction();
	void performOnHoverOutAction();
	void performOnClickAction();

private:
	std::vector<std::shared_ptr<Action>>m_actions;

};


#endif
