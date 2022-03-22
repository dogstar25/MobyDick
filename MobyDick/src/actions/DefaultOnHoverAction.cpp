#include "DefaultOnHoverAction.h"


#include "../GameObject.h"

void DefaultOnHoverAction::perform(GameObject* gameObject)
{
	const auto& renderComponent = gameObject->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	renderComponent->applyDisplayOverlay(displayOverlays::outline_GREEN1);


}
