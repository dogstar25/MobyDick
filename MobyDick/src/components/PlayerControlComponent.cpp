#include "PlayerControlComponent.h"

#include <iostream>

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
#include "ActionComponent.h"
#include "../GameObject.h"

#include <SDL2/SDL.h>


PlayerControlComponent::PlayerControlComponent()
{

}

PlayerControlComponent::PlayerControlComponent(Json::Value definitionJSON, GameObject* gameObject) :
	Component(gameObject)
{

	Json::Value componentJSON = definitionJSON["playerControlComponent"];

	for (Json::Value itrControls : componentJSON["controls"])
	{
		int controlFlag = EnumMap::instance().toEnum(itrControls.asString());
		m_controls.set(controlFlag);


	}


}

PlayerControlComponent::~PlayerControlComponent()
{

}



void PlayerControlComponent::init()
{

	m_controls.reset();

	



	
}


void PlayerControlComponent::update()
{

	if (m_controls.test(CONTROL_MOVEMENT))
	{
		handleMovement();
	}

	handleActions();

}


void PlayerControlComponent::handleMovement()
{
	float angularVelocity = 0;
	int direction = 0;
	int strafe = 0;
	const Uint8* keyStates = nullptr;

	for (auto& inputEvent : EventManager::instance().playerInputEvents())
	{
		direction = 0;
		strafe = 0;
		keyStates = inputEvent->keyStates;

		switch (inputEvent->event.type)
		{
		case SDL_KEYUP:
		case SDL_KEYDOWN:
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

			m_gameObject->actionComponent()->moveAction(direction, strafe);

			break;

		case SDL_MOUSEMOTION:
			m_gameObject->actionComponent()->rotateAction();
			angularVelocity = inputEvent->event.motion.xrel * GameConfig::instance().mouseSensitivity();
			m_gameObject->physicsComponent()->applyRotation(angularVelocity);
			break;
		default:
			break;
		}

	}

}

void PlayerControlComponent::handleActions()
{
	const Uint8* keyStates = nullptr;

	for (auto& inputEvent : EventManager::instance().playerInputEvents())
	{
		keyStates = inputEvent->keyStates;

		switch (inputEvent->event.type)
		{
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			if (keyStates[SDL_SCANCODE_G])
			{
				//actionMap["DROP_WEAPON"]->perform();
				std::cout << "Dropped Weapon" <<"\n";
			}

		case SDL_MOUSEBUTTONDOWN:
			//Execute USE
			if (m_controls.test(CONTROL_USE))
			{
				//actionMap["USE"]->perform();
			}

			break;
		default:
			break;
		}

	}
}



