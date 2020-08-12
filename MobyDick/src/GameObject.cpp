#include "GameObject.h"

#include <iostream>

#include "GameObjectManager.h"
#include "components/TransformComponent.h"
#include "components/InventoryComponent.h"

#include "Level.h"
#include "Scene.h"
#include "game.h"


GameObject::~GameObject()
{

}

GameObject::GameObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust)
{

	//Game Object Id
	m_id = gameObjectId;
	m_removeFromWorld = false;

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
		component.second->setActive(true);
	}

	//SetRenderComponent dependencies
	getComponent<RenderComponent>()->setDependencyReferences(this);

}

void GameObject::setPosition(b2Vec2 position, float angle)
{
	//-1 means dont apply the angle
	if (angle != -1)
	{
		getComponent<TransformComponent>()->setPosition(position, angle);
	}
	else
	{
		getComponent<TransformComponent>()->setPosition(position);
	}

}


void GameObject::update()
{

	if (this->m_id.compare("GINA_64") == 0) {
		std::cout << "Updateing GINA" << "\n";
	}


	for (auto& component : m_components)
	{
		component.second->update();
	}

}



void GameObject::render()
{
	SDL_Rect* textureSourceRect=NULL;
	SDL_FRect destRect;
	SDL_Texture* texture=NULL;


	//Render yourself
	getComponent<RenderComponent>()->render();
		
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

	getComponent<ParticleComponent>()->reset();
	getComponent<PhysicsComponent>()->setOffGrid();

}

void GameObject::addInventoryItem(std::shared_ptr<GameObject>inventoryObject)
{
	size_t itemCount = getComponent<InventoryComponent>()->addItem( inventoryObject);
	//If this is the only iventory item, then attach it to the player of whatever object this is
	if (itemCount == 1)
	{
		getComponent<PhysicsComponent>()->attachItem(inventoryObject);
	}

	//Also add the item to the world
	//Game::instance().addGameObject(inventoryObject, GameObjectLayer::DEBUG);

}

void GameObject::init()
{
	for (auto& component : m_components){

		component.second->setParent(this);

	}

	_setDependecyReferences();



}
