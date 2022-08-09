#include "NavigationComponent.h"
#include "../EnumMap.h"
#include "../RayCastCallBack.h"
#include "../game.h"

extern std::unique_ptr<Game> game;

NavigationComponent::~NavigationComponent()
{ 

}

NavigationComponent::NavigationComponent(Json::Value componentJSON)
{
	m_componentType = ComponentTypes::NAVIGATION_COMPONENT;

	m_type = game->enumMap()->toEnum(componentJSON["type"].asString());

}

void NavigationComponent::update()
{

}

void NavigationComponent::postInit()
{
	//For this GameObject, find all other navigation gameobjects that have been created, 
	// and initilaize the navigation data required
	updateNavObjectsAccess();

}

void NavigationComponent::updateNavObjectsAccess()
{

	m_accessibleNavObjects.clear();

	for (auto& gameObject : parent()->parentScene()->gameObjects()[GameLayer::ABSTRACT])
	{
		if (gameObject->hasComponent(ComponentTypes::NAVIGATION_COMPONENT)) {

			//Ignore the gameObject in the world collection that IS this particular gameObject
			if (parent() != gameObject.get()) {

				b2Vec2 thisGameObjectPosition = { parent()->getCenterPosition().x / GameConfig::instance().scaleFactor(),
					parent()->getCenterPosition().y / GameConfig::instance().scaleFactor() };
				b2Vec2 foundGameObjectPosition = { gameObject->getCenterPosition().x / GameConfig::instance().scaleFactor(),
					gameObject->getCenterPosition().y / GameConfig::instance().scaleFactor() };

				RayCastCallBack::instance().reset();
				parent()->parentScene()->physicsWorld()->RayCast(&RayCastCallBack::instance(), thisGameObjectPosition, foundGameObjectPosition);

				if (RayCastCallBack::instance().hasClearNavPath()) {

					m_accessibleNavObjects.push_back(gameObject);

				}
			}
		}
	}

}