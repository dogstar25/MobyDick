#ifndef ACTION_MAPS_H
#define ACTION_MAPS_H

#include <map>
#include <memory>
#include <string>
#include <assert.h>

#include "Action.h"
#include "UseAction.h"
#include "MoveAction.h"
#include "RotateAction.h"
#include "InteractAction.h"

class ActionMaps
{
public:
	static ActionMaps& instance();
	ActionMaps();
	~ActionMaps();

	std::shared_ptr<Action> getAction(std::string actionKey) {

		assert(m_actionMap.find(actionKey) != m_actionMap.end() && "ActionKey was not found in ActionMaps collection");

		return m_actionMap[actionKey];
	}

private:

	std::map<std::string, std::shared_ptr<Action>>m_actionMap;

};

#endif

