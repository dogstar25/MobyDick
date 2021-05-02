#include "InventoryComponent.h"


InventoryComponent::InventoryComponent()
{

}

InventoryComponent::InventoryComponent(Json::Value definitionJSON, Scene* parentScene)
{

	m_activeItem = 0;

	Json::Value definitionComponentJSON = definitionJSON["inventoryComponent"];

	//for (Json::Value itrItem : definitionComponentJSON["items"])
	//{
	//	std::string gameObjectId = itrItem["gameObjectId"].asString();
	//	auto gameObject = std::make_shared<GameObject>(gameObjectId, -1.0F, -1.0F, 0.F, parentScene);
	//	gameObject->init();

	//	m_items.emplace_back(std::move(gameObject));

	//}

}

InventoryComponent::~InventoryComponent()
{

}

//void InventoryComponent::weldOnAttachments()
//{
//	assert(parent() != nullptr  && "parent GameObject hasn't been initialized");
//
//	for (auto& item : m_items) {
//		if (item.attached) {
//			parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT)->attachItem(item.gameObject.get());
//		}
//	}
//
//}


size_t InventoryComponent::addItem(std::shared_ptr<GameObject> gameObject)
{

	gameObject->init();

	m_items.emplace_back(gameObject);

	return(m_items.size());

}

GameObject* InventoryComponent::getActiveItem() {

	assert(m_activeItem < m_items.size() && "activeItem is out of range of current items");

	return m_items[m_activeItem].get();
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

