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
	auto& inventoryComponent =
		std::static_pointer_cast<InventoryComponent>(gameObject->components()[INVENTORY_COMPONENT]);

	//Get active inventory item 
	if (auto& item = inventoryComponent->items()[inventoryComponent->activeItem()].lock())
	{
		//Get the action component of this item
		auto& itemActionComponent =
			std::static_pointer_cast<ActionComponent>(item->components()[ACTION_COMPONENT]);

		//Perform the UseAction of this item
		itemActionComponent->useAction(item.get());
	}



}