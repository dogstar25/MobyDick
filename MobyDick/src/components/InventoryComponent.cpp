#include "InventoryComponent.h"

#include "../GameObject.h"


InventoryComponent::InventoryComponent()
{

	m_activeItem = 0;

}

InventoryComponent::~InventoryComponent()
{

}

size_t InventoryComponent::addItem(std::shared_ptr<GameObject>gameObject, std::shared_ptr<GameObject>inventoryObject)
{

	m_items.push_back(inventoryObject);

	return(m_items.size());

}
