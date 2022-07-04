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
	inline constexpr int general = 0;
	inline constexpr int boosting = 1;
	inline constexpr int invulnerable = 2;
	inline constexpr int dead = 3;

}

class PlayerControlComponent : public Component
{
public:

	PlayerControlComponent();
	PlayerControlComponent(Json::Value componentJSON);

	~PlayerControlComponent();

	virtual void update() {};
	void setState(int state) { m_currentState = state; }
	int state() { return m_currentState; }

protected:
	int m_currentState{ PlayerState::general };


};


#endif
