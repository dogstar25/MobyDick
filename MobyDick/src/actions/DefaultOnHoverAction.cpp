#include "DefaultOnHoverAction.h"


#include "../GameObject.h"

DefaultOnHoverAction::DefaultOnHoverAction()
{

}

DefaultOnHoverAction::~DefaultOnHoverAction()
{

}

void DefaultOnHoverAction::perform(GameObject* gameObject)
{
	const auto& renderComponent = gameObject->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	renderComponent->applyDisplayOverlay(displayOverlays::outline);


}
