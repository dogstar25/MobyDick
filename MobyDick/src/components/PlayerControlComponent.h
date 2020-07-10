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
	PlayerControlComponent(Json::Value componentJSON, GameObject* gameObject);

	~PlayerControlComponent();

	void update() override;

	void setDependencyReferences(std::shared_ptr<TransformComponent> transformComponent,
		std::shared_ptr<AnimationComponent> animationComponent,
		std::shared_ptr<PhysicsComponent> physicsComponent,
		std::shared_ptr<VitalityComponent> vitalityComponent);



private:

	void init();
	void handleMovement();
	void handleActions();


	std::bitset<8> m_controls;

	std::shared_ptr<TransformComponent> m_refTransformComponent;
	std::shared_ptr<AnimationComponent> m_refAnimationComponent;
	std::shared_ptr<PhysicsComponent> m_refPhysicsComponent;
	std::shared_ptr<VitalityComponent> m_refVitalityComponent;

};


#endif
