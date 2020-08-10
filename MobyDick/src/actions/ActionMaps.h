#ifndef ACTION_MAPS_H
#define ACTION_MAPS_H

#include <map>
#include <memory>
#include <string>

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

	std::shared_ptr<Action> getAction(std::string jsonAction) {
		return m_actionMap[jsonAction];
	}

private:

	std::map<std::string, std::shared_ptr<Action>>m_actionMap;

};

#endif

