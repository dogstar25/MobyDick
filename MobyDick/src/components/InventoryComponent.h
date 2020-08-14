#ifndef INVENTORY_COMPONENT_H
#define INVENTORY_COMPONENT_H

#include <vector>
#include <assert.h>

#include "Component.h"

class GameObject;

class InventoryComponent : public Component
{
public:
	InventoryComponent();
	~InventoryComponent();

	int addItem(GameObject* gameObject);

	std::vector<GameObject*> items() {
		return m_items;
	}

	int activeItem() {
		return m_activeItem;
	}

	GameObject* getActiveItem() {

		assert(m_activeItem < m_items.size() && "activeItem is out of range of current items");

		return m_items[m_activeItem];
	}

private:

	int m_activeItem;
	std::vector<GameObject*> m_items;



};

#endif
