#include "CheckPointComponent.h"
#include "../EnumMap.h"
#include "../RayCastCallBack.h"
#include "../game.h"
#include "../triggers/TriggerFactory.h"

extern std::unique_ptr<Game> game;

CheckPointComponent::~CheckPointComponent()
{ 

}

CheckPointComponent::CheckPointComponent(Json::Value componentJSON)
{
	m_componentType = ComponentTypes::CHECKPOINT_COMPONENT;


	if (componentJSON.isMember("reSpawnLocation")) {

		m_playerReSpawnLocation.x = componentJSON["reSpawnLocation"]["x"].asFloat();
		m_playerReSpawnLocation.y = componentJSON["reSpawnLocation"]["y"].asFloat();
	}

	if (componentJSON.isMember("trigger")) {
		m_trigger = game->triggerFactory()->create(componentJSON["trigger"].asString());
	}

	m_contactRequired = componentJSON["contactRequired"].asBool();
	m_sequence = componentJSON["sequence"].asInt();


}

void CheckPointComponent::update()
{

	bool contactRequirementMet{};
	bool triggerRequirementMet{};

	//No need to check anything if the criteria has already been met
	if (m_hasMetCriteria == false) {

		//Contact Requirement
		if (m_contactRequired == true) {

			if (m_contactMade == true) {
				contactRequirementMet = true;
			}

		}
		else {
			contactRequirementMet = true;
		}

		//Trigger Requirement
		if (m_trigger.has_value()) {

			if (m_trigger.value()->hasMetCriteria(parent()->parentScene()) == true) {

				triggerRequirementMet = true;
				m_trigger.value().get()->execute();

			}

		}
		else {
			triggerRequirementMet = true;
		}

		if (contactRequirementMet && triggerRequirementMet) {

			m_hasMetCriteria = true;
		}
	}
}

void CheckPointComponent::postInit()
{

}

