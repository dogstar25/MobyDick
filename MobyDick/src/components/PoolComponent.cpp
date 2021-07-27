#include "PoolComponent.h"
#include "../GameObject.h"


PoolComponent::PoolComponent(Json::Value definitionJSON)
{
	Json::Value componentJSON = definitionJSON["poolComponent"];

	m_isAvailable = true;
	m_isActive = false;

}

PoolComponent::~PoolComponent()
{

}

void PoolComponent::update()
{

}

void PoolComponent::reset()
{
	m_isActive = false;
	m_isAvailable = true;
	//parent()->setIsPooledAvailable( true);


}


