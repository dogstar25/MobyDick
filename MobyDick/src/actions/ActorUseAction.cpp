#include "ActorUseAction.h"


#include "../components/InventoryComponent.h"

/*
The Actors UseAction is usually to Perform the UsageAction of the Actor's activeItem from his inventory
*/
void ActorUseAction::perform(GameObject* gameObject)
{
	Json::Value actionParms{};

	//Get the Actores inventory component
	const std::shared_ptr<InventoryComponent> inventoryComponent = gameObject->getComponent<InventoryComponent>(ComponentTypes::INVENTORY_COMPONENT);

	//Get active inventory item 
	if (const auto item = inventoryComponent->getActiveItem())
	{
		const auto& actionComponent = item->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT);
		
		auto usageAction = actionComponent->getAction(ACTION_USAGE);
		usageAction->perform(item);

	}



}