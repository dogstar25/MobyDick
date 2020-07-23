#include "HeroUseAction.h"

#include <memory>

#include "../components/InventoryComponent.h"
#include "../components/ActionComponent.h"
#include "../game.h"

HeroUseAction::HeroUseAction()
{

}

HeroUseAction::~HeroUseAction()
{


}

/*
The Heroes UseAction is usually to Perform the UseAction of the Hero's activeItem from his inventory
*/
void HeroUseAction::perform(Entity entity)
{
	//Get the Heroes inventory component
	auto& inventoryComponent = Game::instance().gameCoordinator().GetComponent<InventoryComponent>(entity);

	////Get active inventory item 
	//if (auto& item = inventoryComponent.items()[inventoryComponent.activeItem()].lock())
	//{
	//	//Perform the UseAction of this item
	//	item->getComponent<ActionComponent>()->useAction(item.get());
	//}



}