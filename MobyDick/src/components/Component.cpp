#include "Component.h"

#include "AnimationComponent.h"
#include "ActionComponent.h"
#include "AttachmentsComponent.h"
#include "ChildrenComponent.h"
#include "CompositeComponent.h"
#include "ParticleComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "VitalityComponent.h"
#include "WeaponComponent.h"
#include "PlayerControlComponent.h"

Component::Component()
{

}

Component::~Component()
{

}

void Component::update()
{


}

void Component::setDependencyReferences(std::map<int, std::shared_ptr<Component>>components)
{
	m_refcomponents = components;
}


//bool Component::hasRefComponent(int componentId) {
//
//	auto iter = m_refcomponents.find(componentId);
//
//	if (iter != m_refcomponents.end())
//	{
//		if(iter->second && iter->second->isActive() == true)
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

//std::shared_ptr<Component> Component::getRefComponent(int componentId)
//{
//
//		try
//		{
//			return m_refcomponents.at(componentId);
//		}
//		catch (const std::exception&)
//		{
//			return std::shared_ptr<Component>();
//		}
//
//}
//
