#include "PlayerControlComponent.h"
#include "../GameObjectManager.h"
#include "../Globals.h"
#include "../GameObject.h"
#include "../EventManager.h"
#include "../GameConfig.h"
#include "../EnumMaps.h"


#include "AnimationComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "VitalityComponent.h"

#include <SDL2/SDL.h>


PlayerControlComponent::PlayerControlComponent()
{

}

PlayerControlComponent::PlayerControlComponent(Json::Value definitionJSON)
{

	Json::Value itr = definitionJSON["playerControlComponent"];

	m_parentGameObjectId = definitionJSON["id"].asString();;

	for (Json::Value itrControls :itr["controls"])
	{
		int controlFlag = EnumMap::instance().toEnum(itrControls.asString());
		m_controls.set(controlFlag);


	}


}

PlayerControlComponent::~PlayerControlComponent()
{

}

void PlayerControlComponent::setDependencyReferences(std::shared_ptr<TransformComponent> transformComponent,
	std::shared_ptr<AnimationComponent> animationComponent,
	std::shared_ptr<PhysicsComponent> physicsComponent,
	std::shared_ptr<VitalityComponent> vitalityComponent)
{

	m_refTransformComponent = transformComponent;
	m_refAnimationComponent = animationComponent;
	m_refPhysicsComponent = physicsComponent;
	m_refVitalityComponent = vitalityComponent;

}


void PlayerControlComponent::init()
{

	m_controls.reset();
}


void PlayerControlComponent::update()
{

	int keyCode = 0, scanCode, keyCount, keyStateCount;
	float angularVelocity = 0;
	int direction = 0;
	int strafe = 0;
	const Uint8* keyStates = nullptr;

	for (auto& inputEvent : EventManager::instance().playerInputEvents())
	{
		direction = 0;
		strafe = 0;

		switch (inputEvent->event.type)
		{
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			keyStates = inputEvent->keyStates;
			if (keyStates[SDL_SCANCODE_W])
			{
				direction = -1;
			}
			if (keyStates[SDL_SCANCODE_S])
			{
				direction = 1;
			}
			if (keyStates[SDL_SCANCODE_A])
			{
				strafe = 1;
			}
			if (keyStates[SDL_SCANCODE_D])
			{
				strafe = -1;
			}

			//FIXME: moving, sound and animation change shoudl be part of a actionSequence helper class
			if (m_refPhysicsComponent) {
				m_refPhysicsComponent->applyMovement(m_refVitalityComponent->speed(), direction, strafe);
			}
			//Sound
			//playSound(0);

			//Animation
			if (m_refAnimationComponent )
			{
				if (direction == 0 && strafe == 0)
				{
					m_refAnimationComponent->setCurrentAnimationState(ANIMATION_IDLE);
				}
				else
				{
					m_refAnimationComponent->setCurrentAnimationState(ANIMATION_RUN);
				}
			}

			break;

		case SDL_MOUSEMOTION:
			angularVelocity = inputEvent->event.motion.xrel * GameConfig::instance().mouseSensitivity();
			if (m_refPhysicsComponent) {
				m_refPhysicsComponent->applyRotation(angularVelocity);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			//fire();
			break;
		default:
			break;
		}

	}


}
