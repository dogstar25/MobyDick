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

	size_t addItem(GameObject* gameObject);
	std::vector<GameObject*> items() { return m_items; }
	int activeItem() {	return m_activeItem; }

	GameObject* getActiveItem();

private:

	int m_activeItem{ 0 };
	std::vector<GameObject*> m_items;



};

#endif
