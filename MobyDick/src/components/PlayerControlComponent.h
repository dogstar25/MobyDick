#ifndef PLAYER_CONTROL_COMPONENT_H
#define PLAYER_CONTROL_COMPONENT_H

#include <memory>
#include <string>
#include <bitset>
#include <chrono>

#include "Component.h"

class AnimationComponent;
class PhysicsComponent;
class TransformComponent;
class VitalityComponent;


class PlayerControlComponent : public Component
{
public:

	PlayerControlComponent();
	PlayerControlComponent(Json::Value definitionJSON);

	~PlayerControlComponent();

	void update(std::shared_ptr<GameObject>gameObject) override;

	void testParticle();

private:

	void init();
	void handleMovement(std::shared_ptr<GameObject>gameObject);
	void handleActions(std::shared_ptr<GameObject>gameObject);


	std::bitset<8> m_controls;
	std::chrono::steady_clock::time_point move_time_snapshot;
	std::chrono::steady_clock::time_point rotation_time_snapshot;

};


#endif
