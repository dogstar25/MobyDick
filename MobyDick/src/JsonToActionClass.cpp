#include "JsonToActionClass.h"
#include "actions/ActorMoveAction.h"
#include "actions/ActorRotateAction.h"
#include "actions/ActorUseAction.h"
#include "actions/NoAction.h"

JsonToActionClass& JsonToActionClass::instance()
{
	static JsonToActionClass singletonInstance;
	return singletonInstance;
}


JsonToActionClass::JsonToActionClass()
{
	jsonToMoveAction["ActorMove"] = std::make_shared<ActorMoveAction>();
	jsonToRotateAction["ActorRotate"] = std::make_shared<ActorRotateAction>();
	jsonToUseAction["ActorUse"] = std::make_shared<ActorUseAction>();

}

JsonToActionClass::~JsonToActionClass()
{

}