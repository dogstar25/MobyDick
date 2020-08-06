#ifndef ACTION_COMPONENT_H
#define ACTION_COMPONENT_H

#include "Component.h"

#include <map>
#include <json/json.h>

#include <queue>
#include "../actions/Action.h"
#include "../actions/MoveAction.h"
#include "../actions/RotateAction.h"
#include "../actions/InteractAction.h"
#include "../actions/UseAction.h"
#include "../Globals.h"


class GameObject;

class ActionComponent : public Component
{
public:
	ActionComponent(Json::Value definitionJSON);
	~ActionComponent();

	void update();

	std::shared_ptr<MoveAction> buildMoveAction(int direction, int strafe);
	std::shared_ptr<RotateAction> buildRotateAction(float angularVelocity);
	std::shared_ptr<UseAction> buildUseAction();
	std::shared_ptr<InteractAction> buildInteractAction();

	void performMoveAction(int direction, int strafe);
	void performRotateAction(float angularVelocity);
	void performUseAction();
	void performInteractAction();
private:
	
	std::map<int, std::string> m_actions;


};


#endif
