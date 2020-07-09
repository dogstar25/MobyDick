#include "Action.h"
#include "HeroFireAction.h"

void Action::init()
{

	map["HeroFireAction"] = &createInstance<HeroFireAction>;


	//map["HeroFireAction"]();
}


