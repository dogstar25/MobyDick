#ifndef JSON_TO_ACTIONCLASS_H
#define JSON_TO_ACTIONCLASS_H

#include <map>
#include <memory>
#include <string>

#include "actions/Action.h"
#include "actions/HeroFireAction.h"

class JsonToActionClass
{
public:
	static JsonToActionClass& instance();
	JsonToActionClass();
	~JsonToActionClass();

	std::shared_ptr<Action> toClass(std::string jsonAction) {
		return jsonToAction[jsonAction];
	}

private:
	std::map<std::string, std::shared_ptr<Action>>jsonToAction;

};

#endif

