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

namespace PlayerState {
	inline constexpr int invulnerable = 1;
	inline constexpr int dead = 2;

}

class PlayerControlComponent : public Component
{
public:

	PlayerControlComponent();
	PlayerControlComponent(Json::Value componentJSON);

	~PlayerControlComponent();

	virtual void update() {};
	//void setState(int state) { m_currentState = state; }
	std::bitset<8> state() { return m_state; }

	

protected:
	//int m_currentState{ PlayerState::general };
	std::bitset<8> m_state{};

};


#endif
