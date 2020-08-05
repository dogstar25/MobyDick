#ifndef JSON_TO_ACTIONCLASS_H
#define JSON_TO_ACTIONCLASS_H

#include <map>
#include <memory>
#include <string>

#include "actions/Action.h"
#include "actions/UseAction.h"
#include "actions/MoveAction.h"
#include "actions/RotateAction.h"
#include "actions/InteractAction.h"

class JsonToActionClass
{
public:
	static JsonToActionClass& instance();
	JsonToActionClass();
	~JsonToActionClass();

	std::shared_ptr<MoveAction> toMoveClass(std::string jsonAction) {
		return jsonToMoveAction[jsonAction];
	}
	std::shared_ptr<RotateAction> toRotateClass(std::string jsonAction) {
		return jsonToRotateAction[jsonAction];
	}
	std::shared_ptr<UseAction> toUseClass(std::string jsonAction) {
		return jsonToUseAction[jsonAction];
	}
	std::shared_ptr<InteractAction> toInteractClass(std::string jsonAction) {
		return jsonToInteractAction[jsonAction];
	}

private:
	std::map<std::string, std::shared_ptr<MoveAction>>jsonToMoveAction;
	std::map<std::string, std::shared_ptr<RotateAction>>jsonToRotateAction;
	std::map<std::string, std::shared_ptr<UseAction>>jsonToUseAction;
	std::map<std::string, std::shared_ptr<InteractAction>>jsonToInteractAction;

};

#endif

