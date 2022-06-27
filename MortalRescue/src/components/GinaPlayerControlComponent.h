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

enum class PlayerState {
	general = 0,
	boosting,
	invulnerable,
	dead
};

class GinaPlayerControlComponent : public PlayerControlComponent
{
public:

	GinaPlayerControlComponent();
	GinaPlayerControlComponent(Json::Value componentJSON);

	~GinaPlayerControlComponent();

	void update() override;
	void setState(PlayerState state) { m_currentState = state; }
	PlayerState state() { return m_currentState; }
	void boostReset(bool boostTimerRest);
	void postInit() override;


private:

	void handleMovement();
	void handleActions();

	PlayerState m_currentState{ PlayerState::general};

	Timer m_boostTimer{};
	Timer m_boostAgainTimer{};

	void _jetPackSwitch(bool turnOn);
};


