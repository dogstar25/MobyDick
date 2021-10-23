#pragma once
#include "components\WeaponComponent.h"

class PistolWeaponComponent : public WeaponComponent
{

public:

	PistolWeaponComponent(Json::Value definitionJSON);

	void checkLevelUp(const int scrapCount);


private:



};

