#ifndef ACTION_COMPONENT_H
#define ACTION_COMPONENT_H

#include "Component.h"

#include <map>
#include <vector>
#include <optional>

#include <json/json.h>
#include <box2d/box2d.h>

#include "../actions/Action.h"
#include "../actions/MoveAction.h"
#include "../actions/RotateAction.h"
#include "../actions/InteractAction.h"
#include "../actions/UseAction.h"
#include "../actions/NoAction.h"
#include "../actions/LoadNextLevelAction.h"
#include "../Util.h"
#include "../BaseConstants.h"


class GameObject;

class ActionComponent : public Component
{
public:
	ActionComponent(Json::Value definitionJSON);
	~ActionComponent();

	void update();

	void performMoveAction(int direction, int strafe);
	void performMoveAction(b2Vec2 trajectory);
	void performRotateAction(float angularVelocity);
	void performUseAction();
	void performUsageAction();
	void performInteractAction(std::tuple<std::string, int, float> params);
	void performOnHoverAction();
	void performOnHoverOutAction();
	void performOnClickAction();
	void performTriggerAction();

private:
	std::vector<std::shared_ptr<Action>>m_actions;

};


#endif
