#include "BobbyPlayerControlComponent.h"

#include "IMGui/IMGuiUtil.h"
#include "SceneManager.h"
#include "EnumMap.h"
#include "game.h"
#include "../GameConstants.h"

extern std::unique_ptr<Game> game;

BobbyPlayerControlComponent::BobbyPlayerControlComponent()
{

}

BobbyPlayerControlComponent::BobbyPlayerControlComponent(Json::Value componentJSON)
	: PlayerControlComponent(componentJSON)
{

}

BobbyPlayerControlComponent::~BobbyPlayerControlComponent()
{

}

void BobbyPlayerControlComponent::postInit()
{

}

void BobbyPlayerControlComponent::update()
{

	//ALways ensure that bobby is upright
	const auto& playerPhysics = parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	playerPhysics->setAngle(0.);

	_handleActions();

	_handleMovement();

}

void BobbyPlayerControlComponent::_handleMovement()
{
	int mouseX = 0, mouseY = 0;
	int direction = 0, strafe = 0;
	bool onStairs{};

	//convenience reference to outside component(s)
	const auto& actionComponent = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);

	//Handle Keyboard related movement
	const uint8_t* currentKeyStates = SDL_GetKeyboardState(NULL);

	if (_isTouchingStairs()) {

		onStairs = true;
		_applyStairWalkingSettings();
	}
	else {
		onStairs = false;
		_removeStairWalkingSettings();
	}

	if (currentKeyStates[SDL_SCANCODE_W])
	{
		direction = -1;
	}
	if (currentKeyStates[SDL_SCANCODE_S])
	{
		direction = 1;

	}

	if (currentKeyStates[SDL_SCANCODE_A])
	{
		strafe = -1;
	}
	if (currentKeyStates[SDL_SCANCODE_D])
	{
		strafe = 1;
	}
	
	//Sprint
	if (currentKeyStates[SDL_SCANCODE_LSHIFT])
	{

		//const auto& action = actionComponent->getAction(ACTION_SPRINT);
		//action->perform(parent(), direction, strafe);
		//m_state.set(PlayerState::sprinting);
	}
	else {


		if (onStairs) {
			const auto& moveAction = actionComponent->getAction(Actions::STAIRS_MOVE);
			moveAction->perform(parent(), direction);
		}
		if(strafe != 0) {
			const auto& moveAction = actionComponent->getAction(Actions::MOVE);
			moveAction->perform(parent(), direction, strafe);
		}


		//m_state.reset(PlayerState::sprinting);
	}


	//Handle Mouse related movement
	const uint32_t currentMouseStates = SDL_GetRelativeMouseState(&mouseX, &mouseY);
	float angularVelocity = mouseX * game->contextMananger()->getMouseSensitivity();

}


void BobbyPlayerControlComponent::_applyStairWalkingSettings()
{
	const auto& playerPhysics = parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& playerVitality = parent()->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	playerPhysics->setGravityScale(0);
	playerPhysics->setLinearDamping(100);
	playerPhysics->setAngularDamping(100);
	playerVitality->setSpeed(5);

}

void BobbyPlayerControlComponent::_removeStairWalkingSettings()
{

	const auto& playerPhysics = parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& playerVitality = parent()->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

	playerPhysics->setGravityScale(15);
	playerPhysics->setLinearDamping(1);
	playerPhysics->setAngularDamping(1);
	playerVitality->setSpeed(3);

}

void BobbyPlayerControlComponent::_handleActions()
{
	//Get the current mouse state
	int mouseX, mouseY;
	auto mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);

	if (SceneManager::instance().playerInputEvents().empty() == false) {
		//convenience reference to outside component(s)
		const auto& actionComponent = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);

		const Uint8* keyStates = nullptr;
		std::shared_ptr<Action> action{};

		for (auto& inputEvent : SceneManager::instance().playerInputEvents())
		{
			//std::optional<Action> playerAction{};
			keyStates = inputEvent.keyStates;
			SDL_Scancode keyScanCode = SDL_GetScancodeFromKey(inputEvent.event.key.keysym.sym);

			
			switch (inputEvent.event.type)
			{
				//case SDL_KEYUP:
				case SDL_KEYDOWN:

					////Interaction Keys
					//if (keyScanCode == SDL_SCANCODE_E || keyScanCode == SDL_SCANCODE_R)
					//{
					//	action = actionComponent->getAction(ACTION_INTERACT);
					//	action->perform(parent(), keyScanCode);
					//}

					break;
				case SDL_MOUSEBUTTONDOWN:
					
					if(m_state.test(PlayerState::sprinting) == false){

						if (mouseButtons & SDL_BUTTON_LMASK) {
							action = actionComponent->getAction(Actions::USE);
							action->perform(parent(), Actions::USAGE);
						}
					}
					break;

				default:
					break;
			}

		}
	}
}

bool BobbyPlayerControlComponent::_isTouchingStairs()
{

	for (const auto& touchingObject : parent()->getTouchingObjects()) {

		if (touchingObject.second.expired() == false && touchingObject.second.lock()->type() == "MAIN_STAIRS") {
			return true;
		}
	}

	return false;
}



