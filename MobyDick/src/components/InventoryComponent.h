#ifndef INVENTORY_COMPONENT_H
#define INVENTORY_COMPONENT_H

#include <vector>
#include "Component.h"

class GameObject;

class InventoryComponent : public Component
{
public:
	InventoryComponent();
	~InventoryComponent();

	size_t addItem(std::shared_ptr<GameObject>inventoryObject);

	std::vector<std::weak_ptr<GameObject>> items() {
		return m_items;
	}

	size_t activeItem() {
		return m_activeItem;
	}

private:

	size_t m_activeItem;
	std::vector<std::weak_ptr<GameObject>> m_items;



};

#endif
