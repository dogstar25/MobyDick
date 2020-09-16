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
	this->renderComponent = std::make_shared<RenderComponent>(definitionJSON);
	this->transformComponent = std::make_shared<TransformComponent>(definitionJSON, xMapPos, yMapPos, angleAdjust);

	//Animation Component
	if (definitionJSON.isMember("animationComponent"))
	{
		this->animationComponent = std::make_shared<AnimationComponent>(definitionJSON);
	}

	//Physics Component
	if (definitionJSON.isMember("physicsComponent") && definitionJSON.isMember("transformComponent"))
	{
		this->physicsComponent = std::make_shared<PhysicsComponent>(definitionJSON, xMapPos, yMapPos, angleAdjust);
	}

	//Vitality Component
	if (definitionJSON.isMember("vitalityComponent"))
	{
		this->vitalityComponent = std::make_shared<VitalityComponent>(definitionJSON);
	}

	//Player control Component
	if (definitionJSON.isMember("playerControlComponent"))
	{
		this->playerControlComponent = std::make_shared<PlayerControlComponent>(definitionJSON);
	}

	//Text Component
	if (definitionJSON.isMember("textComponent"))
	{
		this->textComponent = std::make_shared<TextComponent>(gameObjectId, definitionJSON);
	}

	//Children Component
	if (definitionJSON.isMember("childrenComponent"))
	{
		this->childrenComponent = std::make_shared<ChildrenComponent>(definitionJSON);
	}

	//Action Component
	if (definitionJSON.isMember("actionComponent"))
	{
		this->actionComponent = std::make_shared<ActionComponent>(definitionJSON);
	}

	//Particle X Component
	if (definitionJSON.isMember("particleXComponent"))
	{
		this->particleXComponent = std::make_shared<ParticleXComponent>(definitionJSON);
	}

	//Simple Particle Component
	if (definitionJSON.isMember("particleComponent"))
	{
		this->particleComponent = std::make_shared<ParticleComponent>(definitionJSON);
	}

	//Inventory Component
	if (definitionJSON.isMember("inventoryComponent"))
	{
		this->inventoryComponent = std::make_shared<InventoryComponent>();
	}

	//UIControl Component
	if (definitionJSON.isMember("UIControlComponent"))
	{
		this->uicontrolComponent = std::make_shared<UIControlComponent>(definitionJSON);
	}

	//Weapon Component
	if (definitionJSON.isMember("weaponComponent"))
	{
		this->weaponComponent = std::make_shared<WeaponComponent>(definitionJSON);
	}

	//Pool Component
	if (definitionJSON.isMember("poolComponent"))
	{
		this->poolComponent = std::make_shared<PoolComponent>(definitionJSON);
	}

	//Composite Component
	if (definitionJSON.isMember("compositeComponent"))
	{
		this->compositeComponent = std::make_shared<CompositeComponent>(definitionJSON);
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
	//renderComponent.value()->setDependencyReferences(this);

}

void GameObject::addComponent(const std::shared_ptr<Component> component, ComponentTypes componentType)
{

		m_components_new[(std::size_t)componentType]=component;

}

std::shared_ptr<Component> GameObject::getComponent(ComponentTypes componentType)
{
	std::shared_ptr<Component> component = nullptr;

	switch (componentType) {

		case ComponentTypes::ACTION_COMPONENT: {
			component =  std::static_pointer_cast<ActionComponent>(m_components_new[(std::size_t)componentType]);
		}
		case ComponentTypes::ANIMATION_COMPONENT: {
			component = std::static_pointer_cast<AnimationComponent>(m_components_new[(std::size_t)componentType]);
		}
		case ComponentTypes::ATTACHMENTS_COMPONENT: {
			component = std::static_pointer_cast<AttachmentsComponent>(m_components_new[(std::size_t)componentType]);
		}
		case ComponentTypes::CHILDREN_COMPONENT: {
			component = std::static_pointer_cast<ChildrenComponent>(m_components_new[(std::size_t)componentType]);
		}
		case ComponentTypes::COMPOSITE_COMPONENT: {
			component = std::static_pointer_cast<CompositeComponent>(m_components_new[(std::size_t)componentType]);
		}
		case ComponentTypes::INVENTORY_COMPONENT: {
			component = std::static_pointer_cast<InventoryComponent>(m_components_new[(std::size_t)componentType]);
		}
		case ComponentTypes::PARTICLE_COMPONENT: {
			component = std::static_pointer_cast<ParticleComponent>(m_components_new[(std::size_t)componentType]);
		}
		case ComponentTypes::PARTICLE_X_COMPONENT: {
			component = std::static_pointer_cast<ParticleXComponent>(m_components_new[(std::size_t)componentType]);
		}
		case ComponentTypes::PHYSICS_COMPONENT: {
			component = std::static_pointer_cast<PhysicsComponent>(m_components_new[(std::size_t)componentType]);
		}
		case ComponentTypes::PLAYERCONTROL_COMPONENT: {
			component = std::static_pointer_cast<PlayerControlComponent>(m_components_new[(std::size_t)componentType]);
		}
		case ComponentTypes::POOL_COMPONENT: {
			component = std::static_pointer_cast<PoolComponent>(m_components_new[(std::size_t)componentType]);
		}
		case ComponentTypes::RENDER_COMPONENT: {
			component = std::static_pointer_cast<RenderComponent>(m_components_new[(std::size_t)componentType]);
		}
		case ComponentTypes::TEXT_COMPONENT: {
			component = std::static_pointer_cast<TextComponent>(m_components_new[(std::size_t)componentType]);
		}
		case ComponentTypes::TRANSFORM_COMPONENT: {
			component = std::static_pointer_cast<TransformComponent>(m_components_new[(std::size_t)componentType]);
		}
		case ComponentTypes::UICONTROL_COMPONENT: {
			component = std::static_pointer_cast<UIControlComponent>(m_components_new[(std::size_t)componentType]);
		}
		case ComponentTypes::VITALITY_COMPONENT: {
			component = std::static_pointer_cast<VitalityComponent>(m_components_new[(std::size_t)componentType]);
		}
		case ComponentTypes::WEAPON_COMPONENT: {
			component = std::static_pointer_cast<WeaponComponent>(m_components_new[(std::size_t)componentType]);
		}
		default:
			assert(false && "Component type is not valid");

	}

	return component;

}

void GameObject::setPosition(float x, float y)
{

	transformComponent.value()->setPosition(x, y);

}


void GameObject::setPosition(b2Vec2 position, float angle)
{
	//-1 means don't apply the angle
	if (angle != -1)
	{
		transformComponent.value()->setPosition(position, angle);
	}
	else
	{
		transformComponent.value()->setPosition(position);
	}

}


void GameObject::update()
{

	/*for (auto& component : m_components_new)
	{
		if (component) {
			component->update();
		}
	}*/

	if (actionComponent) {
		actionComponent.value()->update();
	}
	if (animationComponent) {
		animationComponent.value()->update();
	}
	if (attachmentsComponent) {
		attachmentsComponent.value()->update();
	}
	if (childrenComponent) {
		childrenComponent.value()->update();
	}
	if (compositeComponent) {
		compositeComponent.value()->update();
	}
	if (inventoryComponent) {
		inventoryComponent.value()->update();
	}
	if (particleComponent) {
		particleComponent.value()->update();
	}
	if (particleXComponent) {
		particleXComponent.value()->update();
	}
	if (playerControlComponent) {
		playerControlComponent.value()->update();
	}
	if (physicsComponent) {
		physicsComponent.value()->update();
	}
	if (poolComponent) {
		poolComponent.value()->update();
	}
	if (renderComponent) {
		renderComponent.value()->update();
	}
	if (textComponent) {
		textComponent.value()->update();
	}
	if (transformComponent) {
		transformComponent.value()->update();
	}
	if (uicontrolComponent) {
		uicontrolComponent.value()->update();
	}
	if (vitalityComponent) {
		vitalityComponent.value()->update();
	}
	if (weaponComponent) {
		weaponComponent.value()->update();
	}

}




void GameObject::render()
{

	//Render yourself
	renderComponent.value()->render();
		
	//Render your children
	if (childrenComponent){

		childrenComponent.value()->renderChildren();
	}

	//If you have an non-physics particle emitter then render those particles
	if (particleComponent) {

		particleComponent.value()->render();
	}

	//If you have a composite component, then render the composite pieces
	if (compositeComponent) {

		compositeComponent.value()->render();
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
	if (poolComponent) {
		poolComponent.value()->reset();
	}
	if (physicsComponent) {
		physicsComponent.value()->setOffGrid();
	}
}

void GameObject::addInventoryItem( GameObject* gameObject)
{
	size_t itemCount = inventoryComponent.value()->addItem(gameObject);
	//If this is the only inventory item, then attach it to the player of whatever object this is
	if (itemCount == 1)
	{
		physicsComponent.value()->attachItem(gameObject);
	}

}

void GameObject::init(bool cameraFollow)
{
	/*for (auto& component : m_components_new){

		if (component) {
			component->setParent(this);
		}

	}*/

	if (actionComponent) {
		actionComponent.value()->setParent(this);
	}
	if (animationComponent) {
		animationComponent.value()->setParent(this);
	}
	if (attachmentsComponent) {
		attachmentsComponent.value()->setParent(this);
	}
	if (childrenComponent) {
		childrenComponent.value()->setParent(this);
	}
	if (compositeComponent) {
		compositeComponent.value()->setParent(this);
	}
	if (inventoryComponent) {
		inventoryComponent.value()->setParent(this);
	}
	if (particleComponent) {
		particleComponent.value()->setParent(this);
	}
	if (particleXComponent) {
		particleXComponent.value()->setParent(this);
	}
	if (playerControlComponent) {
		playerControlComponent.value()->setParent(this);
	}
	if (physicsComponent) {
		physicsComponent.value()->setParent(this);
	}
	if (poolComponent) {
		poolComponent.value()->setParent(this);
	}
	if (renderComponent) {
		renderComponent.value()->setParent(this);
	}
	if (textComponent) {
		textComponent.value()->setParent(this);
	}
	if (transformComponent) {
		transformComponent.value()->setParent(this);
	}
	if (uicontrolComponent) {
		uicontrolComponent.value()->setParent(this);
	}
	if (vitalityComponent) {
		vitalityComponent.value()->setParent(this);
	}
	if (weaponComponent) {
		weaponComponent.value()->setParent(this);
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