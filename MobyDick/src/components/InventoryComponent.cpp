#include "InventoryComponent.h"



InventoryComponent::InventoryComponent()
{

	m_activeItem = 0;

}

InventoryComponent::~InventoryComponent()
{

}

size_t InventoryComponent::addItem(GameObject* gameObject)
{

	m_items.push_back(gameObject);

	return(m_items.size());

}

GameObject* InventoryComponent::getActiveItem() {

	assert(m_activeItem < m_items.size() && "activeItem is out of range of current items");

	return m_items[m_activeItem];
}

