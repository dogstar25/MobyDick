#pragma once

#include <memory>
#include <string>
#include <bitset>
#include <chrono>

#include "components/PlayerControlComponent.h"
#include "Timer.h"

class AnimationComponent;
class PhysicsComponent;
class TransformComponent;
class VitalityComponent;


namespace PlayerState {
	inline constexpr int boosting = 5;
	//inline constexpr int chargingWeapon = 6;
}

class GinaPlayerControlComponent : public PlayerControlComponent
{
public:

	GinaPlayerControlComponent();
	GinaPlayerControlComponent(Json::Value componentJSON);

	~GinaPlayerControlComponent();

	void update() override;
	void boostReset(bool boostTimerRest);
	void postInit() override;


private:

	void handleMovement();
	void handleActions();

	Timer m_boostTimer{};
	Timer m_boostAgainTimer{};
	void _disableWeapon();
	void _enableWeapon();

	void _jetPackSwitch(bool turnOn);
	void _sendWeaponChargeFlag(const bool isCharging);
};


