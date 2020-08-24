#include "DefaultOnHoverOutAction.h"


#include "../GameObject.h"

DefaultOnHoverOutAction::DefaultOnHoverOutAction()
{

}

DefaultOnHoverOutAction::~DefaultOnHoverOutAction()
{

}

void DefaultOnHoverOutAction::perform(GameObject* gameObject)
{
	const auto& renderComponent = gameObject->getComponent<RenderComponent>();

	renderComponent->removeDisplayScheme();


}

