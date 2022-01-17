#include "AttachmentsComponent.h"

#include "../EnumMaps.h"
#include "../GameObject.h"
#include "InventoryComponent.h"




AttachmentsComponent::AttachmentsComponent(Json::Value componentJSON, Scene* parentScene)
{

	for (Json::Value itrItem : componentJSON["attachments"])
	{
		std::string gameObjectId = itrItem["gameObjectId"].asString();
		bool addToInventory = itrItem["addToInventory"].asBool();
		b2Vec2 attachLocation = { itrItem["attachLocation"]["x"].asFloat(), itrItem["attachLocation"]["y"].asFloat() };
		b2JointType attachB2JointType = static_cast<b2JointType>(EnumMap::instance().toEnum(itrItem["attachB2JointType"].asString()));
		auto gameObject = std::make_shared<GameObject>(gameObjectId, -1.0F, -1.0F, 0.F, parentScene);

		Attachment attachment = { addToInventory, attachB2JointType, attachLocation, std::move(gameObject) };
		m_attachments.emplace_back(attachment);

	}
}

AttachmentsComponent::~AttachmentsComponent()
{

}

void AttachmentsComponent::update()
{

	for (const auto& attachment: m_attachments) {

		attachment.gameObject->update();
	}


}

void AttachmentsComponent::render()
{

	for (const auto& attachment : m_attachments) {

		attachment.gameObject->render();
	}


}

void AttachmentsComponent::postInit()
{


	const auto& physicsComponent = parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

	for (auto& attachment : m_attachments) {

		physicsComponent->attachItem(attachment.gameObject.get(), attachment.attachB2JointType, attachment.attachLocation);
		if (attachment.addToInventory == true) {

			const auto& inventoryComponent = parent()->getComponent<InventoryComponent>(ComponentTypes::INVENTORY_COMPONENT);
			inventoryComponent->addItem(attachment.gameObject);
		}

	}
}

const std::optional<Attachment> AttachmentsComponent::getAttachment(std::string id)
{
	std::optional<Attachment> foundAttachment{};

	for (auto& attachment : m_attachments) {

		if (attachment.gameObject->id() == id) {
			foundAttachment = attachment;
		}
	}

	return foundAttachment;

}
