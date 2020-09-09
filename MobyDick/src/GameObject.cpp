#include "GameObject.h"


#include "GameObjectManager.h"
#include "components/InventoryComponent.h"

#include "Camera.h"
#include "EnumMaps.h"


GameObject::~GameObject()
{

}

GameObject::GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust)
	: sf::Sprite()
{

	Json::Value definitionJSON;

	//Build components
	if (gameObjectId.rfind("DEBUG_", 0) != 0)
	{
		definitionJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();
	}
	else
	{
		definitionJSON = GameObjectManager::instance().getDefinition("DEBUG_ITEM")->definitionJSON();
	}

	//Category and Id
	m_id = gameObjectId;
	m_idTag = EnumMap::instance().toEnum(definitionJSON["idTag"].asString());
	
	m_removeFromWorld = false;

	//Always build a render and transform component
	addComponent(std::make_shared<RenderComponent>(definitionJSON));
	addComponent(std::make_shared<TransformComponent>(definitionJSON, xMapPos, yMapPos, angleAdjust));

	//Animation Component
	if (definitionJSON.isMember("animationComponent"))
	{
		addComponent(std::make_shared<AnimationComponent>(definitionJSON));
	}

	//Physics Component
	if (definitionJSON.isMember("physicsComponent") && definitionJSON.isMember("transformComponent"))
	{
		addComponent(std::make_shared<PhysicsComponent>(definitionJSON, xMapPos, yMapPos, angleAdjust));
	}

	//Vitality Component
	if (definitionJSON.isMember("vitalityComponent"))
	{
		addComponent(std::make_shared<VitalityComponent>(definitionJSON));
	}

	//Player control Component
	if (definitionJSON.isMember("playerControlComponent"))
	{
		addComponent(std::make_shared<PlayerControlComponent>(definitionJSON));
	}

	//Text Component
	if (definitionJSON.isMember("textComponent"))
	{
		addComponent(std::make_shared<TextComponent>(gameObjectId, definitionJSON));
	}

	//Children Component
	if (definitionJSON.isMember("childrenComponent"))
	{
		addComponent(std::make_shared<ChildrenComponent>(definitionJSON));
	}

	//Action Component
	if (definitionJSON.isMember("actionComponent"))
	{
		addComponent(std::make_shared<ActionComponent>(definitionJSON));
	}

	//Particle Component
	if (definitionJSON.isMember("particleComponent"))
	{
		addComponent(std::make_shared<ParticleComponent>(definitionJSON));
	}

	//Inventory Component
	if (definitionJSON.isMember("inventoryComponent"))
	{
		addComponent(std::make_shared<InventoryComponent>());
	}

	//UIControl Component
	if (definitionJSON.isMember("UIControlComponent"))
	{
		addComponent(std::make_shared<UIControlComponent>(definitionJSON));
	}

	//Weapon Component
	if (definitionJSON.isMember("weaponComponent"))
	{
		addComponent(std::make_shared<WeaponComponent>(definitionJSON));
	}

	//Pool Component
	if (definitionJSON.isMember("poolComponent"))
	{
		addComponent(std::make_shared<PoolComponent>(definitionJSON));
	}

}

//void GameObject::addComponent(int componentId, std::shared_ptr<Component> component)
//{
//
//
//
//
//}


void GameObject::_setDependecyReferences()
{

	//SetRenderComponent dependencies
	getComponent<RenderComponent>()->setDependencyReferences(this);

}

void GameObject::setPosition(float x, float y)
{

	sf::Sprite::setPosition(x, y);

}


void GameObject::setPosition(sf::Vector2f position, float angle)
{

	sf::Sprite::setPosition(position);

	//-1 means dont apply the angle
	if (angle != -1)
	{
		sf::Sprite::setRotation(angle);
	}
	
}


void GameObject::update()
{

	for (auto& component : m_components)
	{
		component.second->update();
	}

}


void GameObject::render()
{

	//Render yourself
	getComponent<RenderComponent>()->render();

	//Render the particle component if this is a particle emitter object
	getComponent<ParticleComponent>()->render();
		
	//Render your children
	if (getComponent<ChildrenComponent>())
	{
		getComponent<ChildrenComponent>()->renderChildren();
	}

}

//bool GameObject::hasComponent(int componentId) {
//
//	auto iter = m_components.find(componentId);
//
//	if (iter != m_components.end())
//	{
//		if (iter->second && iter->second->isActive() == true)
//		{
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//
//	}
//	else
//	{
//		return false;
//	}
//}
//
//std::shared_ptr<Component> GameObject::getRefComponent(int componentId)
//{
//
//	if (hasComponent(componentId))
//	{
//		return m_components.at(componentId);
//	}
//	else
//	{
//		return std::shared_ptr<Component>();
//	}
//
//}

void GameObject::reset()
{
	if (hasComponent<PoolComponent>()) {
		getComponent<PoolComponent>()->reset();
	}
	if (hasComponent<PhysicsComponent>()) {
		getComponent<PhysicsComponent>()->setOffGrid();
	}
	

}

void GameObject::addInventoryItem( GameObject* gameObject)
{
	size_t itemCount = getComponent<InventoryComponent>()->addItem(gameObject);
	//If this is the only iventory item, then attach it to the player of whatever object this is
	if (itemCount == 1)
	{
		getComponent<PhysicsComponent>()->attachItem(gameObject);
	}

}

void GameObject::init(bool cameraFollow)
{
	for (auto& component : m_components){

		component.second->setParent(this);

	}

	_setDependecyReferences();

	if (cameraFollow) {
		Camera::instance().setFollowMe(this);
	}

}


void GameObject::setIsPooledAvailable(int isPooledAvailable)
{
	m_isPooledAvailable = isPooledAvailable;
}