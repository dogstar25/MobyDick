#include "InventoryComponent.h"


InventoryComponent::InventoryComponent()
{

}

InventoryComponent::InventoryComponent(Json::Value definitionJSON)
{

	m_activeItem = 0;

	Json::Value definitionComponentJSON = definitionJSON["inventoryComponent"];

	for (Json::Value itrItem : definitionComponentJSON["items"])
	{
		std::string gameObjectId = itrItem["gameObjectId"].asString();
		bool attach = itrItem["attach"].asBool();
		bool weapon = itrItem["weapon"].asBool();
		const auto& gameObject = std::make_shared<GameObject>(gameObjectId, -1.0F, -1.0F, 0.F);
		gameObject->init();

		InventoryItem inventoryItem = { true,true,std::move(gameObject) };
		m_items.emplace_back(inventoryItem);

	}

}

InventoryComponent::~InventoryComponent()
{

}

void InventoryComponent::weldOnAttachments()
{
	assert(parent() != nullptr  && "parent GameObject hasn't been initialized");

	for (auto& item : m_items) {
		if (item.attached) {
			parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT)->attachItem(item.gameObject.get());
		}
	}

}


size_t InventoryComponent::addItem(GameObject* gameObject)
{

	//m_items.push_back(gameObject);

	return(m_items.size());

}

GameObject* InventoryComponent::getActiveItem() {

	assert(m_activeItem < m_items.size() && "activeItem is out of range of current items");

	return m_items[m_activeItem].gameObject.get();
}

void InventoryComponent::render()
{

	for (auto& item : m_items)
	{
		if (item.attached) {
			item.gameObject->render();
		}
	}

}

void InventoryComponent::update()
{
	//Have each inventory game object update itself
	for (auto& item : m_items)
	{
		item.gameObject->update();
	}

}

