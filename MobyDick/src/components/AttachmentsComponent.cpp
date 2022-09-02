#include "AttachmentsComponent.h"

#include <format>
#include "../EnumMap.h"
#include "../GameObject.h"
#include "../game.h"
#include "../EnumMap.h"

extern std::unique_ptr<Game> game;


AttachmentsComponent::AttachmentsComponent(Json::Value componentJSON, std::string parentName, Scene* parentScene)
{

	m_componentType = ComponentTypes::ATTACHMENTS_COMPONENT;

	int attachmentCount{};
	for (Json::Value itrItem : componentJSON["attachments"])
	{
		attachmentCount++;

		std::string id = itrItem["id"].asString();
		std::string gameObjectType = itrItem["gameObjectType"].asString();
		bool addToInventory = itrItem["addToInventory"].asBool();
		b2Vec2 attachLocation = { itrItem["attachLocation"]["x"].asFloat(), itrItem["attachLocation"]["y"].asFloat() };
		b2JointType attachB2JointType = static_cast<b2JointType>(game->enumMap()->toEnum(itrItem["attachB2JointType"].asString()));

		m_isDependentObjectOwner = true;

		//tOdO: PUT A CHECK HER THAT WILL NOT ALLOW A STATIC OBJECT BE ATTACHED TO A DYNAMIC OBJECT

		std::string name = _buildAttachmentName(parentName, attachmentCount);
		auto gameObject = std::make_shared<GameObject>(gameObjectType, - 1.0F, -1.0F, 0.F, parentScene, 0., false, name);

		//Add index 
		parentScene->addGameObjectIndex(gameObject);

		Attachment attachment = { id, addToInventory, attachB2JointType, attachLocation, gameObject };
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

	_removeFromWorldPass();

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

		//set layer from parent
		attachment.gameObject->setLayer(parent()->layer());

		physicsComponent->attachItem(attachment.gameObject.get(), attachment.attachB2JointType, attachment.attachLocation);
		if (attachment.addToInventory == true) {

			const auto& inventoryComponent = parent()->getComponent<InventoryComponent>(ComponentTypes::INVENTORY_COMPONENT);
			inventoryComponent->addItem(attachment.gameObject);
		}

		attachment.gameObject->postInit();

	}
}

const std::optional<Attachment> AttachmentsComponent::getAttachment(std::string id)
{
	std::optional<Attachment> foundAttachment{};

	for (auto& attachment : m_attachments) {

		if (attachment.id == id) {
			foundAttachment = attachment;
		}
	}

	return foundAttachment;

}

void AttachmentsComponent::setParent(GameObject* parentObject)
{
	//Call base setParent
	Component::setParent(parentObject);

	for (auto& attachment : m_attachments) {

		attachment.gameObject->setParent(parentObject);

	}


}
void AttachmentsComponent::_removeFromWorldPass()
{
	//First remove any pieces that were mared to be removed
	auto it = m_attachments.begin();
	while (it != m_attachments.end()) {

		if (it->gameObject->removeFromWorld() == true) {

			//Remove object from gloabl index collection
			parent()->parentScene()->deleteIndex(it->gameObject->id());

			//it->pieceObject->reset();
			std::cout << "Erased from Attachments collection " << it->gameObject->id() << std::endl;
			it = m_attachments.erase(it);
		}
		else {
			++it;
		}
	}

	m_attachments.shrink_to_fit();

}

void AttachmentsComponent::removeAllAttachments()
{
	for (auto& attachment : m_attachments) {
		attachment.gameObject->setRemoveFromWorld(true);

	}
}

void AttachmentsComponent::removeAttachment(std::string id)
{

	const auto& attachment = getAttachment(id);
	auto& attachmentGameObject = attachment.value().gameObject;

	attachmentGameObject->setRemoveFromWorld(true);

}

std::string AttachmentsComponent::_buildAttachmentName(std::string parentName, int attachmentCount)
{

	auto name = std::format("{}_AT{:03}", parentName.c_str(), attachmentCount);

	return name;

}
