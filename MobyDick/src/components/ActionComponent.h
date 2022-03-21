#ifndef ACTION_COMPONENT_H
#define ACTION_COMPONENT_H

#include "Component.h"

#include <map>
#include <vector>
#include <optional>

#include "../actions/Action.h"


class GameObject;

class ActionComponent : public Component
{
public:
	ActionComponent(Json::Value definitionJSON);
	~ActionComponent();

	void update();

	std::shared_ptr<Action> getAction(int actionId);

private:
	std::vector<std::shared_ptr<Action>>m_actions;

};


#endif
