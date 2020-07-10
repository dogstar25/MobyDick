#include "JsonToActionClass.h"
#include "actions/HeroFireAction.h"
#include "actions/HeroMoveAction.h"
#include "actions/HeroRotateAction.h"

JsonToActionClass& JsonToActionClass::instance()
{
	static JsonToActionClass singletonInstance;
	return singletonInstance;
}


JsonToActionClass::JsonToActionClass()
{

	jsonToAction["HeroFire"] = std::make_shared<HeroFireAction>();
	jsonToAction["HeroMove"] = std::make_shared<HeroMoveAction>();
	jsonToAction["HeroRotate"] = std::make_shared<HeroRotateAction>();


}

JsonToActionClass::~JsonToActionClass()
{

}