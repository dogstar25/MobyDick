#include "PistolWeaponComponent.h"

#include "components/WeaponComponent.h"
#include "components/InventoryComponent.h"
#include "../GameConstants.h"
#include "GameObject.h"
#include "game.h"

extern std::unique_ptr<Game> game;



PistolWeaponComponent::PistolWeaponComponent(Json::Value definitionJSON)
	: WeaponComponent(definitionJSON)
{

	//Give the contextManager the max level for this weapon - probably mainly for the HUD
	auto maxLevel = m_weaponLevelDetails.size();
	auto& pistolStatusItem = game->contextMananger()->getStatusItem("PLAYER_WEAPON_LEVEL");
	pistolStatusItem.setMaxValue(maxLevel);

	//Give the contextManager the upgrade interval value for the max for this weapon
	auto maxIntervalValue = m_weaponLevelDetails.size();
	auto& accrualStatusItem = game->contextMananger()->getStatusItem("PLAYER_WEAPON_LEVEL_ACCRUAL");
	accrualStatusItem.setMaxValue(maxIntervalValue);

}

void PistolWeaponComponent::checkLevelUp(const int scrapCount)
{

	if (scrapCount >= (m_currentLevel * m_levelUpIncrement)) {
		_levelUp();
		game->contextMananger()->setStatusItemValue("PLAYER_WEAPON_LEVEL", m_currentLevel);
	}

	//Update the accrual amount used by things like the hud
	//Only if we are NOT at the maz level already
	if (m_currentLevel < m_weaponLevelDetails.size()) {
		m_currentLevelUpAccrual = scrapCount - (m_levelUpIncrement * (m_currentLevel - 1));
	}
	else {
		m_currentLevelUpAccrual = 0;
	}

	game->contextMananger()->setStatusItemValue("PLAYER_WEAPON_LEVEL_ACCRUAL", m_currentLevelUpAccrual);

}
