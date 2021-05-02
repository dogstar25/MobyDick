#ifndef INVENTORY_COMPONENT_H
#define INVENTORY_COMPONENT_H

#include <vector>
#include <assert.h>
#include <memory>

#include "Component.h"
#include "../GameObject.h"

class InventoryComponent : public Component
{
public:
	InventoryComponent();
	InventoryComponent(Json::Value definitionJSON, Scene* parentScene);
	~InventoryComponent();

	size_t addItem(std::shared_ptr<GameObject> gameObject);
	std::vector<std::shared_ptr<GameObject>> items() { return m_items; }
	int activeItem() {	return m_activeItem; }

	GameObject* getActiveItem();
	void render();
	void update();

private:

	int m_activeItem{ 0 };
	std::vector<std::shared_ptr<GameObject>> m_items;



};

#endif
