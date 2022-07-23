#pragma once
#include "components\WeaponComponent.h"
#include "Timer.h"

class PistolWeaponComponent : public WeaponComponent
{

public:

	PistolWeaponComponent(Json::Value definitionJSON);

	void checkLevelUp(const int scrapCount);
	void charge(bool isCharging);
	void fireSecondary(const b2Vec2& origin, const float& angle);


private:

	Timer m_chargeTimer{};
	bool m_isFullyCharged{};
	
	bool m_isCharging{};
	bool m_chargingSoundStarted{};
	int m_chargingSoundChannel{};
	
	int m_secondaryFireRadius{};
	std::string m_secondaryFireBulletPoolId{};
	float m_secondaryFireForce{};
	SDL_Color m_secondaryFireColor{};

};

