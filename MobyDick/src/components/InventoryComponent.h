#ifndef INVENTORY_COMPONENT_H
#define INVENTORY_COMPONENT_H

#include <vector>
#include <assert.h>
#include <memory>

#include "Component.h"
#include "../GameObject.h"

struct InventoryItem 
{
	bool attached{ false };
	bool isWeapon{ false };
	std::shared_ptr<GameObject> gameObject;

	InventoryItem(bool attached, bool weapon, std::shared_ptr<GameObject> gameObject) :
		attached(attached),
		isWeapon(weapon),
		gameObject(gameObject) {}
};

class InventoryComponent : public Component
{
public:
	InventoryComponent();
	InventoryComponent(Json::Value definitionJSON);
	~InventoryComponent();

	size_t addItem(GameObject* gameObject);
	std::vector<InventoryItem> items() { return m_items; }
	int activeItem() {	return m_activeItem; }

	GameObject* getActiveItem();
	void render();
	void update();
	void weldOnAttachments();

private:

	int m_activeItem{ 0 };
	std::vector<InventoryItem> m_items;



};

#endif
