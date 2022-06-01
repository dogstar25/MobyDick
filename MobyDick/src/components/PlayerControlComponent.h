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

class PlayerControlComponent : public Component
{
public:

	PlayerControlComponent();
	PlayerControlComponent(Json::Value componentJSON);

	~PlayerControlComponent();

	virtual void update() {};



};


#endif
