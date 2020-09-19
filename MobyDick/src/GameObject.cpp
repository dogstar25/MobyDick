#include "GameObject.h"


#include "GameObjectManager.h"
#include "components/InventoryComponent.h"

#include "Camera.h"
#include "EnumMaps.h"


GameObject::~GameObject()
{

}

GameObject::GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust)
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
	addComponent(std::make_shared<RenderComponent>(definitionJSON), ComponentTypes::RENDER_COMPONENT);
	addComponent(std::make_shared<TransformComponent>(definitionJSON, xMapPos, yMapPos, angleAdjust), ComponentTypes::TRANSFORM_COMPONENT);

	//Animation Component
	if (definitionJSON.isMember("animationComponent"))
	{
		addComponent(std::make_shared<AnimationComponent>(definitionJSON), ComponentTypes::ANIMATION_COMPONENT);
	}

	//Physics Component
	if (definitionJSON.isMember("physicsComponent") && definitionJSON.isMember("transformComponent"))
	{
		addComponent(std::make_shared<PhysicsComponent>(definitionJSON, xMapPos, yMapPos, angleAdjust), ComponentTypes::PHYSICS_COMPONENT);
	}

	//Vitality Component
	if (definitionJSON.isMember("vitalityComponent"))
	{
		addComponent(std::make_shared<VitalityComponent>(definitionJSON), ComponentTypes::VITALITY_COMPONENT);
	}

	//Player control Component
	if (definitionJSON.isMember("playerControlComponent"))
	{
		addComponent(std::make_shared<PlayerControlComponent>(definitionJSON), ComponentTypes::PLAYERCONTROL_COMPONENT);
	}

	//Text Component
	if (definitionJSON.isMember("textComponent"))
	{
		addComponent(std::make_shared<TextComponent>(gameObjectId, definitionJSON), ComponentTypes::TEXT_COMPONENT);
	}

	//Children Component
	if (definitionJSON.isMember("childrenComponent"))
	{
		addComponent(std::make_shared<ChildrenComponent>(definitionJSON), ComponentTypes::CHILDREN_COMPONENT);
	}

	//Action Component
	if (definitionJSON.isMember("actionComponent"))
	{
		addComponent(std::make_shared<ActionComponent>(definitionJSON), ComponentTypes::ACTION_COMPONENT);
	}

	//Particle X Component
	if (definitionJSON.isMember("particleXComponent"))
	{
		addComponent(std::make_shared<ParticleXComponent>(definitionJSON), ComponentTypes::PARTICLE_X_COMPONENT);
	}

	//Simple Particle Component
	if (definitionJSON.isMember("particleComponent"))
	{
		addComponent(std::make_shared<ParticleComponent>(definitionJSON), ComponentTypes::PARTICLE_COMPONENT);
	}

	//Inventory Component
	if (definitionJSON.isMember("inventoryComponent"))
	{
		addComponent(std::make_shared<InventoryComponent>(), ComponentTypes::INVENTORY_COMPONENT);
	}

	//UIControl Component
	if (definitionJSON.isMember("UIControlComponent"))
	{
		addComponent(std::make_shared<UIControlComponent>(definitionJSON), ComponentTypes::UICONTROL_COMPONENT);
	}

	//Weapon Component
	if (definitionJSON.isMember("weaponComponent"))
	{
		addComponent(std::make_shared<WeaponComponent>(definitionJSON), ComponentTypes::WEAPON_COMPONENT);
	}

	//Pool Component
	if (definitionJSON.isMember("poolComponent"))
	{
		addComponent(std::make_shared<PoolComponent>(definitionJSON), ComponentTypes::POOL_COMPONENT);
	}

	//Composite Component
	if (definitionJSON.isMember("compositeComponent"))
	{
		addComponent(std::make_shared<CompositeComponent>(definitionJSON), ComponentTypes::COMPOSITE_COMPONENT);
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
	//if (hasComponent(ComponentTypes::RENDER_COMPONENT)) {
	//	getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT)->setDependencyReferences(this);
	//}

}

void GameObject::setPosition(float x, float y)
{

	getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT)->setPosition(x, y);

}


void GameObject::setPosition(b2Vec2 position, float angle)
{
	//-1 means don't apply the angle
	if (angle != -1)
	{
		getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT)->setPosition(position, angle);
	}
	else
	{
		getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT)->setPosition(position);
	}

}


void GameObject::update()
{

	for (auto& component : m_components)
	{
		if (component) {
			component->update();
		}
	}

}




void GameObject::render()
{

	//Render yourself
	getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT)->render();
		
	//Render your children
	if (hasComponent(ComponentTypes::CHILDREN_COMPONENT)){

		getComponent<ChildrenComponent>(ComponentTypes::CHILDREN_COMPONENT)->renderChildren();
	}

	//If you have an arcade particle emitter then render those particles
	if (hasComponent(ComponentTypes::PARTICLE_COMPONENT)) {

		getComponent<ParticleComponent>(ComponentTypes::PARTICLE_COMPONENT)->render();
	}

	//If you have a composite component, then render the composite pieces
	if (hasComponent(ComponentTypes::COMPOSITE_COMPONENT)) {

		getComponent<CompositeComponent>(ComponentTypes::COMPOSITE_COMPONENT)->render();
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
	if (hasComponent(ComponentTypes::POOL_COMPONENT)) {
		getComponent<PoolComponent>(ComponentTypes::POOL_COMPONENT)->reset();
	}
	if (hasComponent(ComponentTypes::PHYSICS_COMPONENT)) {
		getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT)->setOffGrid();
	}
	

}

void GameObject::addInventoryItem( GameObject* gameObject)
{
	size_t itemCount = getComponent<InventoryComponent>(ComponentTypes::INVENTORY_COMPONENT)->addItem(gameObject);
	//If this is the only inventory item, then attach it to the player of whatever object this is
	if (itemCount == 1)
	{
		getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT)->attachItem(gameObject);
	}

}

void GameObject::init(bool cameraFollow)
{
	for (auto& component : m_components){
		if (component) {
			component->setParent(this);
		}

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