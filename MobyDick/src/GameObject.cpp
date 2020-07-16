#include "GameObject.h"

#include "GameObjectManager.h"
#include "EventManager.h"
#include "components/TransformComponent.h"
#include "components/InventoryComponent.h"

#include "Level.h"
#include "game.h"


GameObject::~GameObject()
{

}

GameObject::GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust)
{

	//Game Object Id
	m_id = gameObjectId;
	m_removeFromWorld = false;
	std::shared_ptr<Component>tempPtr;

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

	//Always build a render and transform component
	tempPtr = std::make_shared<RenderComponent>(definitionJSON);
	addComponent(RENDER_COMPONENT, tempPtr);
	tempPtr = std::make_shared<TransformComponent>(definitionJSON, xMapPos, yMapPos, angleAdjust);
	addComponent(TRANSFORM_COMPONENT, tempPtr);

	//Animation Component
	if (definitionJSON.isMember("animationComponent"))
	{
		tempPtr = std::make_shared<AnimationComponent>(definitionJSON);
		addComponent(ANIMATION_COMPONENT, tempPtr);

	}

	//Physics Component
	if (definitionJSON.isMember("physicsComponent") && definitionJSON.isMember("transformComponent"))
	{
		tempPtr = std::make_shared<PhysicsComponent>(definitionJSON, xMapPos, yMapPos, angleAdjust);
		addComponent(PHYSICS_COMPONENT, tempPtr);


	}
	//Vitality Component
	if (definitionJSON.isMember("vitalityComponent"))
	{
		tempPtr = std::make_shared<VitalityComponent>(definitionJSON);
		addComponent(VITALITY_COMPONENT, tempPtr);
	}

	//Player control Component
	if (definitionJSON.isMember("playerControlComponent"))
	{
		tempPtr = std::make_shared<PlayerControlComponent>(definitionJSON);
		addComponent(PLAYERCONTROL_COMPONENT, tempPtr);

	}

	//Text Component
	if (definitionJSON.isMember("textComponent"))
	{
		tempPtr = std::make_shared<TextComponent>(gameObjectId, definitionJSON);
		addComponent(TEXT_COMPONENT, tempPtr);
		
	}

	//Children Component
	if (definitionJSON.isMember("childrenComponent"))
	{
		tempPtr = std::make_shared<ChildrenComponent>(definitionJSON);
		addComponent(CHILDREN_COMPONENT, tempPtr);

	}

	//Action Component
	if (definitionJSON.isMember("actionComponent"))
	{
		tempPtr = std::make_shared<ActionComponent>(definitionJSON);
		addComponent(ACTION_COMPONENT, tempPtr);

	}

	//Particle Component
	if (definitionJSON.isMember("particleComponent"))
	{
		tempPtr = std::make_shared<ParticleComponent>(definitionJSON);
		addComponent(PARTICLE_COMPONENT, tempPtr);

	}

	//Inventory Component
	if (definitionJSON.isMember("inventoryComponent"))
	{
		tempPtr = std::make_shared<InventoryComponent>();
		addComponent(INVENTORY_COMPONENT, tempPtr);

	}

	//Set dependency references
	_setDependecyReferences();

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
	//Copy all of this game objects components to each of individual components so that each component has a reference 
	// to the other components
	for (auto component : m_components)
	{
		//component.second->setDependencyReferences(m_components);
		if (component) {
			component->setActive(true);

		}
	}

}

void GameObject::setPosition(b2Vec2 position, float angle)
{
	//convenience reference to outside component(s)
	std::shared_ptr<TransformComponent> transformComponent =
		std::static_pointer_cast<TransformComponent>(m_components[TRANSFORM_COMPONENT]);

	//-1 means dont apply the angle
	if (angle != -1)
	{
		transformComponent->setPosition(position, angle);
	}
	else
	{
		transformComponent->setPosition(position);
	}

}


void GameObject::update()
{
	for (auto& component : m_components)
	{
		if (component)
		{
			component->update(shared_from_this());
		}
	}



	////Update the mouse state
	//if (SDL_GetRelativeMouseMode() == SDL_FALSE)
	//{
	//	this->updateMouseState();
	//}

	////This object was clicked, so push whatever event is tied to its onClick event property
	//if (m_mouseState == MOUSE_CLICKED)
	//{
	//	this->onMouseClickEvent();
	//}

	////Loop through any possible child objects and update their
	//// position to reflect parent objects position
	//if (m_definition->hasChildObjects == true)
	//{
	//	updateChildObjects();
	//}

}



void GameObject::render()
{
	SDL_Rect* textureSourceRect=NULL;
	SDL_FRect destRect;
	SDL_Texture* texture=NULL;


	//Render yourself
	std::static_pointer_cast<RenderComponent>(m_components[RENDER_COMPONENT])->render(shared_from_this());
		
	//Render your children

	if (m_components[CHILDREN_COMPONENT])
	{
		std::static_pointer_cast<ChildrenComponent>(m_components[CHILDREN_COMPONENT])->renderChildren();
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

	//convenience reference to outside component(s)
	auto& physicsComponent =
		std::static_pointer_cast<PhysicsComponent>(m_components[PHYSICS_COMPONENT]);
	auto& particleComponent =
		std::static_pointer_cast<ParticleComponent>(m_components[PARTICLE_COMPONENT]);

	particleComponent->reset();
	physicsComponent->setOffGrid();

}

void GameObject::addInventoryItem(std::shared_ptr<GameObject>gameObject, std::shared_ptr<GameObject>inventoryObject)
{
	//convenience reference to outside component(s)
	auto& inventoryComponent =
		std::static_pointer_cast<InventoryComponent>(m_components[INVENTORY_COMPONENT]);
	auto& physicsComponent =
		std::static_pointer_cast<PhysicsComponent>(m_components[PHYSICS_COMPONENT]);

	size_t itemCount = inventoryComponent->addItem( gameObject, inventoryObject);
	//If this is the only iventory item, then attach it to the player of whatever object this is
	if (itemCount == 1)
	{
		physicsComponent->attachItem(inventoryObject);
	}

	//Also add the item to the world
	Game::instance().addGameObject(inventoryObject, GameObjectLayer::DEBUG);

}


