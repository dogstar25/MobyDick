#ifndef PLAYER_CONTROL_COMPONENT_H
#define PLAYER_CONTROL_COMPONENT_H

#include <memory>
#include <string>
#include <bitset>

#include "Component.h"

class AnimationComponent;
class PhysicsComponent;
class TransformComponent;
class VitalityComponent;


class PlayerControlComponent : public Component
{
public:

	PlayerControlComponent();
	PlayerControlComponent(Json::Value definitionJSON, GameObject* gameObject);

	~PlayerControlComponent();

	void update() override;



private:

	void init();
	void handleMovement();
	void handleActions();


	std::bitset<8> m_controls;

};


#endif
