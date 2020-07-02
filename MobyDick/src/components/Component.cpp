#include "Component.h"




Component::Component()
{
	

}

Component::Component(GameObject* parentGameObject)
{
	m_parentGameObject = std::make_shared<GameObject>(parentGameObject);

}

Component::~Component()
{


}


void Component::update()
{


}

void Component::render()
{


}
