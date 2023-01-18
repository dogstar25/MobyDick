#include "BobbyPlayerControlComponent.h"

#include "IMGui/IMGuiUtil.h"
#include "SceneManager.h"
#include "EnumMap.h"
#include "game.h"

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

	handleActions();

	handleMovement();

}

void BobbyPlayerControlComponent::handleMovement()
{
	int mouseX = 0, mouseY = 0;
	int direction = 0, strafe = 0;

	//convenience reference to outside component(s)
	const auto& actionComponent = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);

	//Handle Keyboard related movement
	const uint8_t* currentKeyStates = SDL_GetKeyboardState(NULL);

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

		const auto& action = actionComponent->getAction(ACTION_SPRINT);
		action->perform(parent(), direction, strafe);
		m_state.set(PlayerState::sprinting);
	}
	else {

		const auto& moveAction = actionComponent->getAction(ACTION_MOVE);
		moveAction->perform(parent(), direction, strafe);
		m_state.reset(PlayerState::sprinting);
	}


	//Handle Mouse related movement
	const uint32_t currentMouseStates = SDL_GetRelativeMouseState(&mouseX, &mouseY);
	float angularVelocity = mouseX * game->contextMananger()->getMouseSensitivity();

	const auto& rotateAction = actionComponent->getAction(ACTION_ROTATE);
	rotateAction->perform(parent(), angularVelocity);

}
void BobbyPlayerControlComponent::handleActions()
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

					//Interaction Keys
					if (keyScanCode == SDL_SCANCODE_E || keyScanCode == SDL_SCANCODE_R)
					{
						action = actionComponent->getAction(ACTION_INTERACT);
						action->perform(parent(), keyScanCode);
					}

					break;
				case SDL_MOUSEBUTTONDOWN:
					
					if(m_state.test(PlayerState::sprinting) == false){

						if (mouseButtons & SDL_BUTTON_LMASK) {
							action = actionComponent->getAction(ACTION_USE);
							action->perform(parent(), ACTION_USAGE);
						}
					}
					break;

				default:
					break;
			}

		}
	}
}


