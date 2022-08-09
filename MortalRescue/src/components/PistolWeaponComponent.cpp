#include "PistolWeaponComponent.h"

#include "imgui.h"

#include "components/WeaponComponent.h"
#include "components/InventoryComponent.h"
#include "../GameConstants.h"
#include "GameObject.h"
#include "SoundManager.h"
#include "ColorMap.h"
#include "game.h"

extern std::unique_ptr<Game> game;



PistolWeaponComponent::PistolWeaponComponent(Json::Value definitionJSON)
	: WeaponComponent(definitionJSON)
{

	auto chargeTimerTime = definitionJSON["secondaryFire"]["chargeTime"].asFloat();
	m_chargeTimer = { chargeTimerTime };
	m_secondaryFireRadius = definitionJSON["secondaryFire"]["radius"].asFloat();
	m_secondaryFireBulletPoolId = definitionJSON["secondaryFire"]["bulletPoolId"].asString();
	m_secondaryFireColor = game->colorMap()->toSDLColor(definitionJSON["secondaryFire"]["color"].asString());
	m_secondaryFireForce = definitionJSON["secondaryFire"]["force"].asFloat();


}

void PistolWeaponComponent::checkLevelUp(const int scrapCount)
{

	if (scrapCount >= (m_currentLevel * m_levelUpIncrement)) {
		_levelUp();
		game->contextMananger()->setStatusItemValue(StatusItemId::PLAYER_WEAPON_LEVEL, m_currentLevel);
	}

	//Update the accrual amount used by things like the hud
	//Only if we are NOT at the maz level already
	if (m_currentLevel < m_weaponLevelDetails.size()) {
		m_currentLevelUpAccrual = scrapCount - (m_levelUpIncrement * (m_currentLevel - 1));
	}
	else {
		m_currentLevelUpAccrual = 0;
	}

	game->contextMananger()->setStatusItemValue(StatusItemId::PLAYER_WEAPON_ACCRUAL, m_currentLevelUpAccrual);

}

void PistolWeaponComponent::charge(bool isCharging)
{

	game->contextMananger()->setStatusItemValue(StatusItemId::PLAYER_WEAPON_IS_CHARGED, m_isFullyCharged);
	game->contextMananger()->setStatusItemValue(StatusItemId::PLAYER_WEAPON_CHARGED_PERCENT, (float)0);


	//ImGui::Begin("weaponCharge");
	//ImGui::SetWindowPos(ImVec2(400, 0));
	//ImGui::Value("m_isFullyCharged", m_isFullyCharged);
	//ImGui::Value("percentTargetMet", m_chargeTimer.percentTargetMet()*100);
	//ImGui::Value("timeRemaining", m_chargeTimer.timeRemaining().count());
	////ImGui::Value("hasMet", m_chargeTimer.hasMetTargetDuration());
	//ImGui::End();


	//If the charging timer hasnt even been started, then just bail
	if (m_chargeTimer.isSet() == false) {
		return;
	}

	//If we are fully charged, then stay that way and do noting
	if (m_isFullyCharged == false) {

		if (isCharging) {

			game->contextMananger()->setStatusItemValue(StatusItemId::PLAYER_WEAPON_CHARGED_PERCENT, m_chargeTimer.percentTargetMet() * 100);

			if (m_chargingSoundStarted == false) {
				m_chargingSoundChannel = SoundManager::instance().playSound("SFX_PULSE_CHARGING_1");
				m_chargingSoundStarted = true;
			}

			//If we have met the charging timer time, then set to fully charded
			//otherwise
			if (m_chargeTimer.hasMetTargetDuration()) {
				SoundManager::instance().stopSound(m_chargingSoundChannel);
				SoundManager::instance().playSound("SFX_PULSE_FULLY_CHARGED_1");
				m_isFullyCharged = true;
			}

			//update the contextManagers charge value

		}
		else {

			if (m_chargingSoundStarted == true) {
				SoundManager::instance().stopSound(m_chargingSoundChannel);
			}
			m_chargeTimer.reset();
			m_chargingSoundStarted = false;

		}
	}
	else {
		game->contextMananger()->setStatusItemValue(StatusItemId::PLAYER_WEAPON_CHARGED_PERCENT, (float)100);
	}


}

void PistolWeaponComponent::fireSecondary(const b2Vec2& origin, const float& angle)
{

	if (m_isFullyCharged) {
		//Sound
		SoundManager::instance().playSound("SFX_PULSE_FIRE_3");

		WeaponComponent::fire(origin, angle, m_secondaryFireBulletPoolId, m_secondaryFireForce, m_secondaryFireColor);
		m_isFullyCharged = false;
	}
	else {

		SoundManager::instance().playSound("SFX_PULSE_FAIL_1");

	}


}
