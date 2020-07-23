#include "PlayerControlSystem.h"
#include "../game.h"
#include "../EventManager.h"
#include "../components/PlayerControlComponent.h"
#include "../components/ActionComponent.h"
#include "../GameConfig.h"


void PlayerControlSystem::init()
{
	/*
	Build the component signature for this system
	*/
	Signature signature;
	signature.set(Game::instance().gameCoordinator().GetComponentType<PlayerControlComponent>());
	signature.set(Game::instance().gameCoordinator().GetComponentType<ActionComponent>());

	Game::instance().gameCoordinator().SetSystemSignature<AnimationSystem>(signature);

}

void PlayerControlSystem::update()
{

	const Uint8* keyStates = nullptr;

	/*
	Loop through all saved input event
	*/
	for (auto& inputEvent : EventManager::instance().playerInputEvents())
	{
		int direction = 0;
		int strafe = 0;
		float angularVelocity = 0;
		keyStates = inputEvent->keyStates;

		/*
		Apply player input to all applicabel entities
		*/
		for (auto& entity : mEntities)
		{
			auto& playerControlComponent = Game::instance().gameCoordinator().GetComponent<PlayerControlComponent>(entity);
			auto& actionComponent = Game::instance().gameCoordinator().GetComponent<ActionComponent>(entity);

			//Check Pressed or Released
			if (inputEvent->event.type == SDL_KEYUP ||
				inputEvent->event.type == SDL_KEYDOWN)
			{
				//Move Direction
				if (keyStates[SDL_SCANCODE_W])
				{
					direction = -1;
				}
				else if (keyStates[SDL_SCANCODE_S])
				{
					direction = 1;
				}
				//Strafe direction
				if (keyStates[SDL_SCANCODE_A])
				{
					strafe = 1;
				}
				else if (keyStates[SDL_SCANCODE_D])
				{
					strafe = -1;
				}

				if (keyStates[SDL_SCANCODE_G])
				{
					//actionMap["DROP_WEAPON"]->perform();
				}

				/*
				Call movement action if configured to control movement
				*/
				if (playerControlComponent.m_controls.test(CONTROL_MOVEMENT))
				{
					if (auto action = actionComponent.m_actionMap[ACTION_MOVE])
					{
						action->perform(entity, direction, strafe);
					}
				}

			}
			else if (inputEvent->event.type == SDL_MOUSEBUTTONDOWN)
			{
				//Execute USE
				if (playerControlComponent.m_controls.test(CONTROL_USE))
				{
					if (auto action = actionComponent.m_actionMap[ACTION_USE])
					{
						action->perform(entity);
					}
					//actionComponent->useAction(gameObject.get());

				}
			}
			else if (inputEvent->event.type == SDL_MOUSEMOTION)
			{
				/*
				Call movement action if configured to control movement
				*/
				if (playerControlComponent.m_controls.test(CONTROL_MOVEMENT))
				{
					angularVelocity = inputEvent->event.motion.xrel * GameConfig::instance().mouseSensitivity();
					if (auto action = actionComponent.m_actionMap[ACTION_ROTATE])
					{
						action->perform(entity, angularVelocity);
					}
				}

			}
		}
	}

}


