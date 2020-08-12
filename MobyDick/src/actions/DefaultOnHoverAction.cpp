#include "DefaultOnHoverAction.h"

#include <iostream>

#include "../components//RenderComponent.h"
#include "../GameObject.h"

DefaultOnHoverAction::DefaultOnHoverAction()
{

}

DefaultOnHoverAction::~DefaultOnHoverAction()
{

}

void DefaultOnHoverAction::perform(GameObject* gameObject)
{
	const auto& renderComponent = gameObject->getComponent<RenderComponent>();
	//SDL_Color color = { 0,0,250,255 };
	//renderComponent->outlineObject(1, color);

	renderComponent->applyDisplayScheme(DEFAULT_HOVER_SCHEME);


}
