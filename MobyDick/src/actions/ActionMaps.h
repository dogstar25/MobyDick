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

	std::shared_ptr<MoveAction> getMoveAction(std::string jsonAction) {
		return m_MoveActionMap[jsonAction];
	}
	std::shared_ptr<RotateAction> getRotateAction(std::string jsonAction) {
		return m_RotateActionMap[jsonAction];
	}
	std::shared_ptr<UseAction> getUseAction(std::string jsonAction) {
		return m_UseActionMap[jsonAction];
	}
	std::shared_ptr<InteractAction> getInteractAction(std::string jsonAction) {
		return m_InteractActionMap[jsonAction];
	}

private:
	std::map<std::string, std::shared_ptr<MoveAction>>m_MoveActionMap;
	std::map<std::string, std::shared_ptr<RotateAction>>m_RotateActionMap;
	std::map<std::string, std::shared_ptr<UseAction>>m_UseActionMap;
	std::map<std::string, std::shared_ptr<InteractAction>>m_InteractActionMap;

};

#endif

