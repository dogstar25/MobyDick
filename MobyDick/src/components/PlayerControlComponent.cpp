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

PlayerControlComponent::PlayerControlComponent(Json::Value definitionJSON)
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

	//convenience reference to outside component(s)
	auto& actionComponent =
		std::static_pointer_cast<ActionComponent>(m_refcomponents[ACTION_COMPONENT]);

	float angularVelocity = 0;
	int direction = 0;
	int strafe = 0;
	const Uint8* keyStates = nullptr;

	for (auto& inputEvent : EventManager::instance().playerInputEvents())
	{
		std::chrono::steady_clock::time_point now_time;
		std::chrono::duration<double> time_diff;
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

			actionComponent->moveAction(direction, strafe);
			move_time_snapshot = now_time;

			break;

		case SDL_MOUSEMOTION:

			//check the clock and see if enough time as gone by
			now_time = std::chrono::steady_clock::now();
			time_diff = now_time - rotation_time_snapshot;
			if (time_diff.count() > .03)
			{
				angularVelocity = inputEvent->event.motion.xrel * GameConfig::instance().mouseSensitivity();
				actionComponent->rotateAction(angularVelocity);
				rotation_time_snapshot = now_time;
			}
			break;
		default:
			break;
		}

	}

}

void PlayerControlComponent::handleActions()
{
	//convenience reference to outside component(s)
	auto& actionComponent =
		std::static_pointer_cast<ActionComponent>(m_refcomponents[ACTION_COMPONENT]);

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
				actionComponent->useAction();
				
			}

			break;
		default:
			break;
		}

	}
}



