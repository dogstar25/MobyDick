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
	const auto& renderComponent = gameObject->getComponent<RenderComponent>();
	//renderComponent->applyDisplayOverlay(displayOverlays::outline);


}
