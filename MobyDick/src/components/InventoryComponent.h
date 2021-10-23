#ifndef INVENTORY_COMPONENT_H
#define INVENTORY_COMPONENT_H

#include <vector>
#include <assert.h>
#include <memory>

#include "Component.h"
#include "../GameObject.h"

//Constants
namespace CollectibleTypes {
	static inline constexpr int MAX_COLLECTIBLE_TYPES = 15;
	static inline constexpr int COIN = 0;
}


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
	std::optional<GameObject*> getItem(const int traitTag);
	int addCollectible(const int collectibleType, int count);
	
	const std::array<int, CollectibleTypes::MAX_COLLECTIBLE_TYPES>& collectibles() { return m_collectibles; }
	void render();
	void update();

private:

	int m_activeItem{ 0 };
	std::vector<std::shared_ptr<GameObject>> m_items{};
	std::array<int, CollectibleTypes::MAX_COLLECTIBLE_TYPES> m_collectibles{};



};

#endif
