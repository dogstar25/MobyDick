#include "ContainerComponent.h"
#include "../Util.h"

ContainerComponent::ContainerComponent()
{

}

ContainerComponent::ContainerComponent(Json::Value componentJSON, std::string parentName, Scene* parentScene)
{

	m_componentType = ComponentTypes::CONTAINER_COMPONENT;


	m_capacity = componentJSON["capacity"].asInt();
	if (componentJSON.isMember("refillTimer")) {
		m_doesAutoRefill = true;
		float timerDuration = componentJSON["refillTimer"].asFloat();
		m_refillTimer = Timer(timerDuration, true);
	}

	m_contentItemGameObjectType = componentJSON["gameObjectType"].asString();
	m_contentsItemSpawnForce = componentJSON["spawnForce"].asFloat();
	auto count = componentJSON["count"].asInt();

	for (auto i = 0; i < count;i++) {

		addItem(m_contentItemGameObjectType, m_contentsItemSpawnForce, parentScene, parentName, i, true);
	}



}

ContainerComponent::~ContainerComponent()
{

}


void ContainerComponent::postInit()
{

	const auto containerTransformComponent = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	
	for (auto& item : m_items) {

		const auto& itemPhysicsComponent = item.gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);

		//Set the layer from the parent object
		item.gameObject->setLayer(parent()->layer());

		//Set the location in the middle of the container object
		b2Vec2 containerCenter{ containerTransformComponent->getCenterPosition().x, containerTransformComponent->getCenterPosition().y };

		_setPieceLocationAndForce(containerTransformComponent, item);

		item.gameObject->postInit();
	}


}


void ContainerComponent::update()
{

	for (auto& item : m_items) {

		item.gameObject->update();
	}

	//Animation
	if (parent()->hasComponent(ComponentTypes::ANIMATION_COMPONENT)) {
		const auto& animation = parent()->getComponent<AnimationComponent>(ComponentTypes::ANIMATION_COMPONENT);
		if (isEmpty()) {

			animation->setCurrentAnimationState(ANIMATION_IDLE);
		}
		else {
			animation->setCurrentAnimationState(ANIMATION_ACTIVE);
		}
	}

	//Refill
	if (m_doesAutoRefill == true) {

		//If it not full then regen an item
		//If it is full then reset the timer
		if (isFull() == false) {
			if (m_refillTimer.hasMetTargetDuration()) {

				addItem(m_contentItemGameObjectType, m_contentsItemSpawnForce, parent()->parentScene(), parent()->name(), m_items.size()+(int)1);
				m_refillTimer.reset();
			}
		}
		else {
			m_refillTimer.reset();
		}
	}

	_removeFromWorldPass();

}

void ContainerComponent::render()
{
	for (auto& item : m_items) {

		item.gameObject->render();
	}
}

void ContainerComponent::_removeFromWorldPass()
{
	//First remove any pieces that were mared to be removed
	auto it = m_items.begin();
	while (it != m_items.end()) {

		if (it->gameObject->removeFromWorld() == true) {

			//Remove object from gloabl index collection
			parent()->parentScene()->deleteIndex(it->gameObject->id());

			//it->pieceObject->reset();
			std::cout << "Erased from Containers collection " << it->gameObject->id() << std::endl;
			it = m_items.erase(it);
		}
		else {
			++it;
		}
	}

	m_items.shrink_to_fit();

}
void ContainerComponent::addItem(std::string gameObjectType, float spawnForce, Scene* parentScene, std::string parentName, int itemCount, bool onContainerConstruction)
{

	ContainerItem containerItem{};
	containerItem.spawnForce = spawnForce;

	//Create off screen
	std::string name = _buildItemName(parentName, itemCount);
	auto gameObject = std::make_shared<GameObject>(gameObjectType, (float)-50.0, (float)-50.0, (float)0, parentScene, (float)0., false, name);
	containerItem.gameObject = gameObject;
	parentScene->addGameObjectIndex(gameObject);

	//If this is on the container construction we have to wait and let the postinit set the final destination
	//otherwise we can set it now
	if (onContainerConstruction == false) {
		const auto containerTransformComponent = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
		b2Vec2 containerCenter{ containerTransformComponent->getCenterPosition().x, containerTransformComponent->getCenterPosition().y };
		_setPieceLocationAndForce(containerTransformComponent, containerItem);
	}
	
	m_items.emplace_back(containerItem);

}

void ContainerComponent::_setPieceLocationAndForce(const std::shared_ptr<TransformComponent> containerTransform, ContainerItem containerItem)
{


	float randomNumX = util::generateRandomNumber((float)1, containerTransform->getPositionRect().w / 2);
	float randomNumY = util::generateRandomNumber((float)1, containerTransform->getPositionRect().h / 2);

	//adjust to off center
	b2Vec2 position = { containerTransform->getTopLeftPosition().x + randomNumX, containerTransform->getTopLeftPosition().y + randomNumY };

	b2Vec2 pieceLocation = util::toBox2dPoint(position);

	const auto& itemPhysicsComponent = containerItem.gameObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	itemPhysicsComponent->setTransform(pieceLocation, (float)0);

	//Do we start the object out with a force so that it bounces around
	if (containerItem.spawnForce > 0) {

		
		itemPhysicsComponent->applyImpulse(containerItem.spawnForce, { randomNumX, randomNumY });
	}

}


bool ContainerComponent::isEmpty()
{

	if (m_items.size() > 0) {
		return false;
	}
	else {
		return true;
	}

}

bool ContainerComponent::isFull()
{

	if (m_items.size() >= m_capacity) {
		return true;
	}
	else {
		return false;
	}

}

std::string ContainerComponent::_buildItemName(std::string parentName, int itemCount)
{

	auto name = std::format("{}_CH{:03}", parentName, itemCount);

	return name;

}

