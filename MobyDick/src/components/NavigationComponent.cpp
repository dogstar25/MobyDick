#include "NavigationComponent.h"
#include <iostream>


NavigationComponent::~NavigationComponent()
{ 

}

NavigationComponent::NavigationComponent(Json::Value definitionJSON)
{

	Json::Value definitionComponentJSON = definitionJSON["navigationComponent"];

	//No properties to grab from JSON right now - they get set by whoever is creating a navigation object
	auto isWaypoint = definitionComponentJSON["isWaypoint"].asBool();

}

void NavigationComponent::update()
{

}