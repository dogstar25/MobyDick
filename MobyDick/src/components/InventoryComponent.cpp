#include "InventoryComponent.h"


InventoryComponent::InventoryComponent()
{
	m_isDependentObjectOwner = true;
}

InventoryComponent::InventoryComponent(Json::Value componentJSON, std::string parentName, Scene* parentScene)
{

	m_componentType = ComponentTypes::INVENTORY_COMPONENT;

	m_activeItem = 0;

}

InventoryComponent::~InventoryComponent()
{

}

size_t InventoryComponent::addItem(std::shared_ptr<GameObject> gameObject)
{

	m_items.emplace_back(gameObject);

	return(m_items.size());

}

GameObject* InventoryComponent::getActiveItem() {

	assert(m_activeItem < m_items.size() && "activeItem is out of range of current items");

	return m_items[m_activeItem].get();
}

std::optional<GameObject*> InventoryComponent::getItem(const int traitTag)
{
	std::optional<GameObject*> foundItem{};

	for (auto item : m_items) {

		//This assumes only one item with this trait - returns first one
		if(item->hasTrait(traitTag)){
			foundItem = item.get();
			break;
		}
	}
	return foundItem;
}



int InventoryComponent::addCollectible(const int collectibleType, int count)
{

	m_collectibles.at(collectibleType) += count;

	return m_collectibles.at(collectibleType);
}

void InventoryComponent::render()
{

	//for (auto& item : m_items)
	//{
	//	if (item.attached) {
	//		item.gameObject->render();
	//	}
	//}

}

void InventoryComponent::update()
{
	//Have each inventory game object update itself
	/*for (auto& item : m_items)
	{
		item.gameObject->update();
	}*/

}

