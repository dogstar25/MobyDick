#include "ActorUseAction.h"


#include "../components/InventoryComponent.h"

ActorUseAction::ActorUseAction()
{

}

ActorUseAction::~ActorUseAction()
{


}

/*
The Actors UseAction is usually to Perform the UseAction of the Actor's activeItem from his inventory
*/
void ActorUseAction::perform(GameObject* gameObject)
{
	//Get the Actores inventory component
	const std::shared_ptr<InventoryComponent> inventoryComponent = gameObject->getComponent<InventoryComponent>(ComponentTypes::INVENTORY_COMPONENT);

	//Get active inventory item 
	if (const auto item = inventoryComponent->getActiveItem())
	{
		//Perform the UseAction of this item
		item->getComponent<ActionComponent>(ComponentTypes::ACTION_COMPONENT)->performUsageAction();
	}



}