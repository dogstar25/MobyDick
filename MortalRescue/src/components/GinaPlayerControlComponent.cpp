#include "GinaPlayerControlComponent.h"

#include "PistolWeaponComponent.h"
#include "IMGui/IMGuiUtil.h"
#include "SceneManager.h"
#include "EnumMap.h"
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
		m_state.set(PlayerState::boosting);
		_jetPackSwitch(true);
		_disableWeapon();
	}
	else {

		const auto& moveAction = actionComponent->getAction(ACTION_MOVE);
		moveAction->perform(parent(), direction, strafe);
		m_state.reset(PlayerState::boosting);
		_jetPackSwitch(false);
		_enableWeapon();
	}


	//Handle Mouse related movement
	const uint32_t currentMouseStates = SDL_GetRelativeMouseState(&mouseX, &mouseY);
	float angularVelocity = mouseX * game->contextMananger()->getMouseSensitivity();

	const auto& rotateAction = actionComponent->getAction(ACTION_ROTATE);
	rotateAction->perform(parent(), angularVelocity);

}
void GinaPlayerControlComponent::handleActions()
{
	//Get the current mouse state
	int mouseX, mouseY;
	auto mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);

	//We need to know the state of the right mosue button to see
	//if we send the charging indicator to the weapon
	if (mouseButtons & SDL_BUTTON_RMASK) {

		_sendWeaponChargeFlag(true);

	}
	else {
		_sendWeaponChargeFlag(false);
	}

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
					
					if(m_state.test(PlayerState::boosting) == false){

						if (mouseButtons & SDL_BUTTON_LMASK) {
							action = actionComponent->getAction(ACTION_USE);
							action->perform(parent(), ACTION_USAGE);
						}
					}
					break;
				case SDL_MOUSEBUTTONUP:

					//If this is the right mouse button released, then fire the secondary weapon action
					//Note: the weapon will determine whether it can fire based on whether its charged
					if (m_state.test(PlayerState::boosting) == false && inputEvent.event.button.button == SDL_BUTTON_RIGHT) {

						action = actionComponent->getAction(ACTION_USE);
						action->perform(parent(), ACTION_USAGE_SPECIAL);

					}
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

	const auto& soundComponent = jetPack->gameObject->getComponent<SoundComponent>(ComponentTypes::SOUND_COMPONENT);

	static int channel{};

	if (jetPack.has_value()) {
		if (turnOn && jetPack->gameObject->updateDisabled() == true) {
			jetPack->gameObject->enableUpdate();
			soundComponent->playSound("START_SOUND");
			
			m_jetPackSoundChannel = soundComponent->playSound("OPERATING_SOUND");
			soundComponent->unMuteChannel(m_jetPackSoundChannel);
		}
		else if (turnOn == false && jetPack->gameObject->updateDisabled() == false){
			jetPack->gameObject->disableUpdate();
			if (m_jetPackSoundChannel != -1) {
				soundComponent->stopChannel(m_jetPackSoundChannel);
			}

		}
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


void GinaPlayerControlComponent::_disableWeapon()
{

	const auto& playerAttachmentComponent = parent()->getComponent<AttachmentsComponent>(ComponentTypes::ATTACHMENTS_COMPONENT);

	auto& pistol = playerAttachmentComponent->getAttachment("PISTOL");
	pistol->gameObject->disableCollision();
	pistol->gameObject->disableRender();

}

void GinaPlayerControlComponent::_enableWeapon()
{

	const auto& playerAttachmentComponent = parent()->getComponent<AttachmentsComponent>(ComponentTypes::ATTACHMENTS_COMPONENT);

	auto& pistol = playerAttachmentComponent->getAttachment("PISTOL");
	pistol->gameObject->enableCollision();
	pistol->gameObject->enableRender();

}

void GinaPlayerControlComponent::_sendWeaponChargeFlag(const bool isCharging)
{

	const auto& inventoryComponent = parent()->getComponent<InventoryComponent>(ComponentTypes::INVENTORY_COMPONENT);

	//If the active inventory item is not the pistol/weapon then do nothing
	auto pistol = inventoryComponent->getActiveItem();
	if (pistol->hasTrait(TraitTag::weapon) == true) {

		const auto& weaponComponent = pistol->getComponent<PistolWeaponComponent>(ComponentTypes::WEAPON_COMPONENT);

		weaponComponent->charge(isCharging);

	}

}
