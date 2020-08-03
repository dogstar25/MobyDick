#include "HeroUseAction.h"

#include <memory>

#include "../components/InventoryComponent.h"
#include "../components/ActionComponent.h"
#include "../GameObject.h"

HeroUseAction::HeroUseAction()
{

}

HeroUseAction::~HeroUseAction()
{


}

/*
The Heroes UseAction is usually to Perform the UseAction of the Hero's activeItem from his inventory
*/
void HeroUseAction::perform(GameObject* gameObject)
{
	//Get the Heroes inventory component
	const auto& inventoryComponent = gameObject->getComponent<InventoryComponent>();

	//Get active inventory item 
	if (const auto& item = inventoryComponent->items()[inventoryComponent->activeItem()].lock())
	{
		//Perform the UseAction of this item
		//item->getComponent<ActionComponent>()->useAction(item.get());
		item->getComponent<ActionComponent>()->actionMap()[ACTION_USE]->perform(item.get());
	}



}