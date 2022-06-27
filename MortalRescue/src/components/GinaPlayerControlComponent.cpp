#include "GinaPlayerControlComponent.h"


#include "IMGui/IMGuiUtil.h"
#include "SceneManager.h"
#include "EnumMaps.h"
#include "game.h"

extern std::unique_ptr<Game> game;

GinaPlayerControlComponent::GinaPlayerControlComponent()
{

}

GinaPlayerControlComponent::GinaPlayerControlComponent(Json::Value componentJSON)
	: PlayerControlComponent(componentJSON)
{

}

GinaPlayerControlComponent::~GinaPlayerControlComponent()
{

}

void GinaPlayerControlComponent::postInit()
{

	_jetPackSwitch(false);

}

void GinaPlayerControlComponent::update()
{

	handleActions();

	handleMovement();

}

void GinaPlayerControlComponent::handleMovement()
{
	int mouseX = 0, mouseY = 0;
	int direction = 0, strafe = 0;

	//convenience reference to outside component(s)
	const auto& actionComponent = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);

	//Handle Keyboard related movement
	const uint8_t* currentKeyStates = SDL_GetKeyboardState(NULL);

	//Are we boosting and boost timer is complete stop boosting
	if (m_currentState == PlayerState::boosting && m_boostTimer.hasMetTargetDuration()) {

		boostReset(true);
	}

	//Boost
	if (currentKeyStates[SDL_SCANCODE_LSHIFT])
	{
		//If we're already boosting leave boot timer alone
		if (m_boostAgainTimer.hasMetTargetDuration() || m_boostAgainTimer.firstTime) {

			_jetPackSwitch(true);
			const auto& action = actionComponent->getAction(ACTION_SPRINT);
			action->perform(parent());
			m_boostTimer = { 1.5 };
			m_currentState = PlayerState::boosting;
		}

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

	//Dont execute move stuff if we are in boosting state
	if (m_currentState != PlayerState::boosting) {
		const auto& moveAction = actionComponent->getAction(ACTION_MOVE);
		moveAction->perform(parent(), direction, strafe);
	}

	//Handle Mouse related movement
	const uint32_t currentMouseStates = SDL_GetRelativeMouseState(&mouseX, &mouseY);
	float angularVelocity = mouseX * game->contextMananger()->getMouseSensitivity();

	const auto& rotateAction = actionComponent->getAction(ACTION_ROTATE);
	rotateAction->perform(parent(), angularVelocity);

}
void GinaPlayerControlComponent::handleActions()
{

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

					//Interaction Keys
					if (keyScanCode == SDL_SCANCODE_E || keyScanCode == SDL_SCANCODE_R)
					{
						action = actionComponent->getAction(ACTION_INTERACT);
						action->perform(parent(), keyScanCode);
					}

					break;
				case SDL_MOUSEBUTTONDOWN:

					action = actionComponent->getAction(ACTION_USE);
					action->perform(parent());

					break;
				default:
					break;
			}

		}
	}
}


void GinaPlayerControlComponent::_jetPackSwitch(bool turnOn)
{

	const auto& attachmentsComponent = parent()->getComponent<AttachmentsComponent>(ComponentTypes::ATTACHMENTS_COMPONENT);
	auto& jetPack = attachmentsComponent->getAttachment("JETPACK");
	if (turnOn) {
		jetPack->gameObject->enableUpdate();
	}
	else {
		jetPack->gameObject->disableUpdate();
	}

}

void GinaPlayerControlComponent::boostReset(bool boostTimerRest)
{

	m_currentState = PlayerState::general;
	_jetPackSwitch(false);

	//Set a timer so that we cant boost again for a set time
	if (boostTimerRest) {
		m_boostAgainTimer = { 2 };
	}

}

/*
Use this version of handleMovement when you are dealing with controlling multiple objects at the same time
with PlayerControlComponent - Override with this version of PlayerControlComponent at the game level - really only the mouse logic 
*/
//void PlayerControlComponent::handleMovement()
//{
//
//	//convenience reference to outside component(s)
//	const auto& actionComponent = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
//
//	const Uint8* keyStates = nullptr;
//
//	for (auto& inputEvent : SceneManager::instance().playerInputEvents())
//	{
//		int direction = 0;
//		int strafe = 0;
//		keyStates = inputEvent.keyStates;
//
//		switch (inputEvent.event.type)
//		{
//		case SDL_KEYUP:
//		case SDL_KEYDOWN:
//		{
//			if (keyStates[SDL_SCANCODE_W])
//			{
//				direction = -1;
//			}
//			if (keyStates[SDL_SCANCODE_S])
//			{
//				direction = 1;
//			}
//			if (keyStates[SDL_SCANCODE_A])
//			{
//				strafe = 1;
//			}
//			if (keyStates[SDL_SCANCODE_D])
//			{
//				strafe = -1;
//			}
//
//			std::cout << "HandleMovement " << direction << " " << strafe << "\sn";
//			actionComponent->performMoveAction(direction, strafe);
//
//			break;
//		}
//		case SDL_MOUSEMOTION:
//		{
//			auto angularVelocity = inputEvent.event.motion.xrel * GameConfig::instance().mouseSensitivity();
//			actionComponent->performRotateAction(angularVelocity);
//
//			break;
//		}
//		default:
//			break;
//		}
//
//	}
//
//}





