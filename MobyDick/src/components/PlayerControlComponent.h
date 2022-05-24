#ifndef PLAYER_CONTROL_COMPONENT_H
#define PLAYER_CONTROL_COMPONENT_H

#include <memory>
#include <string>
#include <bitset>
#include <chrono>

#include "Component.h"
#include "../Timer.h"

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

class PlayerControlComponent : public Component
{
public:

	PlayerControlComponent();
	PlayerControlComponent(Json::Value componentJSON);

	~PlayerControlComponent();

	void update() override;


private:

	void init();
	void handleMovement();
	void handleActions();
	//std::optional<Action> getKeyAction();

	PlayerState m_currentState{ PlayerState::general};
	std::bitset<8> m_controls;

	Timer m_boostTimer{};

	void _jetPackSwitch(bool turnOn);
};


#endif
