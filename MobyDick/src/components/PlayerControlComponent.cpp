#include "PlayerControlComponent.h"
#include "../GameObjectManager.h"
#include "../Globals.h"
#include "../GameObject.h"
#include "../EventManager.h"
#include "../GameConfig.h"
#include "../EnumMaps.h"

#include <SDL2/SDL.h>


PlayerControlComponent::PlayerControlComponent()
{

}

PlayerControlComponent::PlayerControlComponent(std::string gameObjectId, std::shared_ptr<GameObject> parentGameObject)
{

	Json::Value itrJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();

	//Save the pointer to parent GameObject
	m_parentGameObject = parentGameObject;

	//PlayerControlComponent
	if (itrJSON.isMember("playerControlComponent") && itrJSON.isMember("physicsComponent") && itrJSON.isMember("vitalityComponent"))
	{
		m_parentGameObject->setComponentFlag(PLAYERCONTROL_COMPONENT);

		Json::Value itr = itrJSON["playerControlComponent"];

		for (Json::Value itrControls :itr["controls"])
		{
			int controlFlag = EnumMap::instance().toEnum(itrControls.asString());
			m_controls.set(controlFlag);


		}

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

	int keyCode = 0, scanCode, keyCount, keyStateCount;
	float angularVelocity = 0;
	int direction = 0;
	int strafe = 0;
	const Uint8* keyStates = nullptr;

	//Convenience references
	PhysicsComponent* physicsComponent = &m_parentGameObject->physicsComponent();
	VitalityComponent* vitalityComponent = &m_parentGameObject->vitalityComponent();

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

			//physicsComponent->applyMovement(vitalityComponent->speed(), direction, strafe);
			physicsComponent->applyMovement(10, direction, strafe);

			break;

		case SDL_MOUSEMOTION:
			angularVelocity = inputEvent->event.motion.xrel * GameConfig::instance().mouseSensitivity();
			physicsComponent->applyRotation(angularVelocity);
			break;
		case SDL_MOUSEBUTTONDOWN:
			//fire();
			break;
		default:
			break;
		}

	}


}
