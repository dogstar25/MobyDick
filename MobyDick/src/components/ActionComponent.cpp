#include "ActionComponent.h"
#include "../GameObject.h"
#include "../actions/HeroFireAction.h"
#include "../JsonToActionClass.h"
#include "../EnumMaps.h"

ActionComponent::ActionComponent(Json::Value definitionJSON)
{
	Json::Value componentJSON = definitionJSON["actionComponent"];

	for (Json::Value itrAction: componentJSON["actions"])
	{
		//Get the name of the class to be used as the action as a string
		std::string stringActionClass = itrAction["actionClass"].asString();

		//Get the Enum that reprsents the Game Objects action as an int
		int action = EnumMap::instance().toEnum(itrAction["actionId"].asString());

		//Get the actual action Class object and put it in the map for this particular gameObject action
		//i.e. This objects "USE" actionClass could be "FireBullet" or it could be "HealPlayer"
		m_actionMap.emplace(action, JsonToActionClass::instance().toClass(stringActionClass));

	}


}

ActionComponent::~ActionComponent()
{

}

void ActionComponent::moveAction(int direction, int strafe)
{
	//convenience reference to outside component(s)
	auto& physicsComponent =
		std::static_pointer_cast<PhysicsComponent>(m_refcomponents[PHYSICS_COMPONENT]);
	auto& animationComponent =
		std::static_pointer_cast<AnimationComponent>(m_refcomponents[ANIMATION_COMPONENT]);
	auto& vitalityComponent =
		std::static_pointer_cast<VitalityComponent>(m_refcomponents[VITALITY_COMPONENT]);

	m_actionMap[ACTION_MOVE]->perform(physicsComponent, animationComponent, vitalityComponent->speed(), direction, strafe);
}

void ActionComponent::rotateAction(float angularVelocity)
{
	//convenience reference to outside component(s)
	auto& physicsComponent =
		std::static_pointer_cast<PhysicsComponent>(m_refcomponents[PHYSICS_COMPONENT]);
	auto& animationComponent =
		std::static_pointer_cast<AnimationComponent>(m_refcomponents[ANIMATION_COMPONENT]);

	m_actionMap[ACTION_ROTATE]->perform(physicsComponent, animationComponent, angularVelocity);
}

void ActionComponent::useAction()
{
	//convenience reference to outside component(s)
	auto& physicsComponent =
		std::static_pointer_cast<PhysicsComponent>(m_refcomponents[PHYSICS_COMPONENT]);
	auto& animationComponent =
		std::static_pointer_cast<AnimationComponent>(m_refcomponents[ANIMATION_COMPONENT]);

	m_actionMap[ACTION_USE]->perform();
}




