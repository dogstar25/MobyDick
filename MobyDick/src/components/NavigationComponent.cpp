#include "NavigationComponent.h"
#include "../EnumMaps.h"
#include <iostream>


NavigationComponent::~NavigationComponent()
{ 

}

NavigationComponent::NavigationComponent(Json::Value definitionJSON)
{

	Json::Value definitionComponentJSON = definitionJSON["navigationComponent"];

	m_type = EnumMap::instance().toEnum(definitionComponentJSON["type"].asString());

}

void NavigationComponent::update()
{

}

void NavigationComponent::postInit()
{
	//const auto& navComponent = getComponent<NavigationComponent>(ComponentTypes::NAVIGATION_COMPONENT);
	const auto& transformComponent = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	//For this GameObject, find all other navigation gameobjects that have been created, 
	// and initilaize the navigation data required
	for (auto& gameObject : parent()->parentScene()->gameObjects()[LAYER_ABSTRACT])
	{

		if (gameObject->hasComponent(ComponentTypes::NAVIGATION_COMPONENT)) {

			//Ignore the gameObject in the world collection that IS this particular gameObject
			if (parent() != gameObject.get()) {

				const auto& foundTransformComponent = gameObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
				//Game::instance().physicsWorld()->RayCast(&m_b2RayCastCallback, position, newPosition);
				//SceneManager::instance().scenes().back()
				b2Vec2 thisGameObjectPosition = { transformComponent->getCenterPosition().x / GameConfig::instance().scaleFactor(),
					transformComponent->getCenterPosition().y / GameConfig::instance().scaleFactor() };
				b2Vec2 foundGameObjectPosition = { foundTransformComponent->getCenterPosition().x / GameConfig::instance().scaleFactor(),
					foundTransformComponent->getCenterPosition().y / GameConfig::instance().scaleFactor() };

				RayCastCallBack::instance().reset();
				parent()->parentScene()->physicsWorld()->RayCast(&RayCastCallBack::instance(), thisGameObjectPosition, foundGameObjectPosition);

				if (RayCastCallBack::instance().hasClearNavPath()) {

					m_accessibleNavObjects.push_back(gameObject);

				}


			}



		}



	}



}