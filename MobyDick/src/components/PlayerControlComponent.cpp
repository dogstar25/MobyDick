#include "PlayerControlComponent.h"

#include <iostream>
#include <typeindex>

#include "../GameObjectManager.h"
#include "../SceneManager.h"
#include "../Globals.h"
#include "../GameObject.h"
#include "../GameConfig.h"
#include "../EnumMaps.h"

#include "AnimationComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "VitalityComponent.h"
#include "ActionComponent.h"
#include "../actions/MoveAction.h"
#include "../actions/RotateAction.h"
#include "../actions/UseAction.h"
#include "../GameObject.h"
#include "../ParticleEmission.h"
#include "../ParticleMachine.h"
#include "../Scene.h"

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
	const auto& actionComponent = parent()->getComponent<ActionComponent>();

	float angularVelocity = 0;
	int direction = 0;
	int strafe = 0;
	const Uint8* keyStates = nullptr;

	for (auto& inputEvent : SceneManager::instance().playerInputEvents())
	{
		std::chrono::steady_clock::time_point now_time;
		std::chrono::duration<double> time_diff;
		direction = 0;
		strafe = 0;
		keyStates = inputEvent.keyStates;

		switch (inputEvent.event.type)
		{
		case SDL_KEYUP:
		case SDL_KEYDOWN:
		{
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

			actionComponent->performMoveAction(direction, strafe);

			break;
		}
		case SDL_MOUSEMOTION:
		{
			angularVelocity = inputEvent.event.motion.xrel * GameConfig::instance().mouseSensitivity();
			actionComponent->performRotateAction(angularVelocity);

			break;
		}
		default:
			break;
		}

	}

}

void PlayerControlComponent::handleActions()
{
	//convenience reference to outside component(s)
	const auto& actionComponent = parent()->getComponent<ActionComponent>();

	const Uint8* keyStates = nullptr;

	for (auto& inputEvent : SceneManager::instance().playerInputEvents())
	{
		keyStates = inputEvent.keyStates;

		switch (inputEvent.event.type)
		{
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			if (keyStates[SDL_SCANCODE_G])
			{
				//actionMap["DROP_WEAPON"]->perform();
				std::cout << "Dropped Weapon" <<"\n";
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			//Execute USE
			if (m_controls.test(CONTROL_USE))
			{
				actionComponent->performUseAction();
			}

			break;
		default:
			break;
		}

	}
}



