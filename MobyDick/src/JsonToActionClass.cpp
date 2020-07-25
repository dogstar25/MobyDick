#include "JsonToActionClass.h"
#include "actions/HeroFireAction.h"
#include "actions/HeroMoveAction.h"
#include "actions/HeroRotateAction.h"
#include "actions/HeroUseAction.h"
#include "actions/NoAction.h"

JsonToActionClass& JsonToActionClass::instance()
{
	static JsonToActionClass singletonInstance;
	return singletonInstance;
}


JsonToActionClass::JsonToActionClass()
{
	jsonToAction["NoAction"] = std::make_shared<NoAction>();
	jsonToAction["HeroFire"] = std::make_shared<HeroFireAction>();
	jsonToAction["HeroMove"] = std::make_shared<HeroMoveAction>();
	jsonToAction["HeroRotate"] = std::make_shared<HeroRotateAction>();
	jsonToAction["HeroUse"] = std::make_shared<HeroUseAction>();

}

JsonToActionClass::~JsonToActionClass()
{

}