#include "DefaultOnHoverOutAction.h"


#include "../GameObject.h"


void DefaultOnHoverOutAction::perform(GameObject* gameObject)
{
	const auto& renderComponent = gameObject->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);

	renderComponent->removeDisplayOverlay();


}

