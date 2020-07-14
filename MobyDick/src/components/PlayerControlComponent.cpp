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
#include "../ParticleEmission.h"
#include "../ParticleMachine.h"

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


void PlayerControlComponent::update(std::shared_ptr<GameObject>gameObject)
{

	if (m_controls.test(CONTROL_MOVEMENT))
	{
		handleMovement(gameObject);
	}

	handleActions(gameObject);

}


void PlayerControlComponent::handleMovement(std::shared_ptr<GameObject>gameObject)
{

	//convenience reference to outside component(s)
	auto& actionComponent =
		std::static_pointer_cast<ActionComponent>(gameObject->components()[ACTION_COMPONENT]);

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

			actionComponent->moveAction(gameObject.get(), direction, strafe);

			break;

		case SDL_MOUSEMOTION:

			//check the clock and see if enough time as gone by
			now_time = std::chrono::steady_clock::now();
			time_diff = now_time - rotation_time_snapshot;
			if (time_diff.count() > .03)
			{
				angularVelocity = inputEvent->event.motion.xrel * GameConfig::instance().mouseSensitivity();
				actionComponent->rotateAction(gameObject.get(), angularVelocity);
				rotation_time_snapshot = now_time;
			}
			break;
		default:
			break;
		}

	}

}

void PlayerControlComponent::handleActions(std::shared_ptr<GameObject>gameObject)
{
	//convenience reference to outside component(s)
	auto& actionComponent =
		std::static_pointer_cast<ActionComponent>(gameObject->components()[ACTION_COMPONENT]);

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
			break;
		case SDL_MOUSEBUTTONDOWN:
			//Execute USE
			testParticle();
			if (m_controls.test(CONTROL_USE))
			{
				actionComponent->useAction(gameObject.get());
				
			}

			break;
		default:
			break;
		}

	}
}

void PlayerControlComponent::testParticle()
{

	//use the collision point for the particle emission
	float x = 4;
	float y = 4;
	b2Vec2 particleOrigin = { x,y };

	//temp color code
	SDL_Color colorMin = { 0,0,0,255 };
	SDL_Color colorMax = { 225,255,255,255 };


	ParticleEmission* particleEmission = new ParticleEmission(
		"SMOKE1_POOL",
		particleOrigin, //min position
		particleOrigin,	//max position
		5,	//Force Min
		15,	//force Max
		1.55,	//Lifetime Min
		1.55,	//Lifetime Max
		true,	// Alpha fade
		0,	//Angle min
		360,	//Angle Max
		12,	//Size Min
		32,	//Size Max
		colorMin,	//Color Min
		colorMax,	//Color Max
		30,	//Particle count min
		60	//Particle count max
	);
	ParticleMachine::instance().add(particleEmission);

}

