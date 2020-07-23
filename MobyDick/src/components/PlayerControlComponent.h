#ifndef PLAYER_CONTROL_COMPONENT_H
#define PLAYER_CONTROL_COMPONENT_H

#include <memory>
#include <bitset>
#include <json/json.h>
#include "Component.h"

class AnimationComponent;
class PhysicsComponent;
class TransformComponent;
class VitalityComponent;


struct PlayerControlComponent
{
	std::bitset<8> m_controls;

	PlayerControlComponent() {}
	PlayerControlComponent(Json::Value definitionJSON);

};


#endif
