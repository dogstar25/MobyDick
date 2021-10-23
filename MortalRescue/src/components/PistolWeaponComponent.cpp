#include "PistolWeaponComponent.h"

#include "components/WeaponComponent.h"
#include "components/InventoryComponent.h"
#include "../GameConstants.h"
#include "GameObject.h"



PistolWeaponComponent::PistolWeaponComponent(Json::Value definitionJSON)
	: WeaponComponent(definitionJSON)
{


}


void PistolWeaponComponent::checkLevelUp(const int scrapCount)
{
	
	if (scrapCount >= m_weaponLevelDetails.at(m_currentLevel).levelUpTarget) {
		_levelUp();
	}

}
