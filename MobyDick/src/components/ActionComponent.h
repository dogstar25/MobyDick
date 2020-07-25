#ifndef ACTION_COMPONENT_H
#define ACTION_COMPONENT_H

#include "Component.h"

#include <map>
#include <json/json.h>

#include <queue>
#include "../actions/Action.h"
#include "../Globals.h"


class GameObject;

class ActionComponent : public Component
{
public:
	ActionComponent(Json::Value definitionJSON);
	~ActionComponent();

	std::shared_ptr<Action> getAction(size_t actionId);
	void addAction(std::shared_ptr<Action> action);
	void update(std::shared_ptr<GameObject>gameObject);

	//Accessor functions
	std::map<int, std::shared_ptr<Action>> actionMap() {
		return m_actionMap;
	}

private:
	std::map<int, std::shared_ptr<Action>>m_actionMap;


};


#endif
