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

	std::shared_ptr<Action> getAction(size_t actionId);
	void addAction(std::shared_ptr<Action> action);
	void update();

	void perform(MoveAction action);
	void perform(RotateAction action);
	void perform(UseAction action);
	void perform(InteractAction action);


private:
	
	std::shared_ptr<MoveAction> m_moveAction;
	std::shared_ptr<RotateAction> m_rotateAction;
	std::shared_ptr<UseAction> m_useAction;
	std::shared_ptr<InteractAction> m_interactAction;
	
	
	std::map<int, std::shared_ptr<Action>>m_actionMap;







};


#endif
