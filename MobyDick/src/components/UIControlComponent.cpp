#include "UIControlComponent.h"


#include "../SceneManager.h"


UIControlComponent::UIControlComponent()
{

}

UIControlComponent::~UIControlComponent()
{

}

UIControlComponent::UIControlComponent(Json::Value componentJSON)
{

	m_componentType = ComponentTypes::UICONTROL_COMPONENT;

	for (Json::Value itrControls : componentJSON["controls"])
	{
		/*int controlFlag = EnumMap::instance().toEnum(itrControls.asString());
		m_controls.set(controlFlag);*/
	}

}

void UIControlComponent::update()
{
	//convenience reference to outside component(s)
	const auto& actionComponent = parent()->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
	const auto& transformComponent = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	const Uint8* keyStates = nullptr;
	int mouseX;
	int mouseY;

	//Get gameObject position rect
	SDL_Rect gameObjectPosition;
	gameObjectPosition.x = (int)transformComponent->getPositionRect().x;
	gameObjectPosition.y = (int)transformComponent->getPositionRect().y;
	gameObjectPosition.w = (int)transformComponent->getPositionRect().w;
	gameObjectPosition.h = (int)transformComponent->getPositionRect().h;

	//Get current mouse position
	const uint32_t currentMouseStates = SDL_GetMouseState(&mouseX, &mouseY);
	SDL_Point mouseLocation[1] = { mouseX , mouseY };
	bool mouseIsOnGameObject = SDL_PointInRect(mouseLocation, &gameObjectPosition);

	//Handle OnHover
	if (mouseIsOnGameObject) {
		const auto& action = actionComponent->getAction(ACTION_ON_HOVER);
		action->perform(parent());
	}
	else {
		const auto& action = actionComponent->getAction(ACTION_ON_HOVER_OUT);
		action->perform(parent());
	}

	for (auto& inputEvent : SceneManager::instance().playerInputEvents())
	{
		switch (inputEvent.event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			{
				SDL_Point mouseClick[1] = { inputEvent.event.button.x , inputEvent.event.button.y };
				if (SDL_PointInRect(mouseClick, &gameObjectPosition)) {
					const auto& action = actionComponent->getAction(ACTION_ON_CLICK);
					action->perform();
				}
			}

			break;
		default:
			break;
		}

	}


}

