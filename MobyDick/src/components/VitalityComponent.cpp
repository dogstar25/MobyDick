#include "VitalityComponent.h"

#include "../GameObject.h"


VitalityComponent::VitalityComponent()
{

}


VitalityComponent::VitalityComponent(Json::Value definitionJSON)
{

	Json::Value vitalityComponentJSON = definitionJSON["vitalityComponent"];

	m_speed = vitalityComponentJSON["speed"].asFloat();
	m_lifetime = std::chrono::duration<float>(vitalityComponentJSON["lifetime"].asFloat());
	if (m_lifetime <= std::chrono::duration<float>(0)) {
		m_hasFiniteLifetime = false;
	}
	else {
		m_hasFiniteLifetime = true;
	}

	m_lifetimeRemaining = std::chrono::duration<float>(m_lifetime);
	m_lifeTimeTimeSnapshot = std::chrono::steady_clock::now();
	m_isLifetimeAlphaFade = vitalityComponentJSON["lifetimeAlphaFade"].asBool();

	//Regeneration related
	if (vitalityComponentJSON.isMember("regenerating")) {
		m_isRegenerative = true;
		auto& regenJSON = vitalityComponentJSON["regenerating"];

			m_regenSpeed = regenJSON["regenerateSpeed"].asFloat();
			m_resistance = regenJSON["resistance"].asFloat();

			auto level = 0;
			for (Json::Value itrlevel : regenJSON["levels"]) {

				LevelDefinition levelItem = {};
				levelItem.levelNum = itrlevel["level"].asInt();
				levelItem.resistance = itrlevel["resistance"].asInt();
				levelItem.color = util::JsonToColor(itrlevel["color"]);

				m_regenLevels[level] = levelItem;
				level++;
			}

		m_maxLevels = level;

		//Set values for first level
		m_resistance = m_regenLevels[0].resistance;
		m_currentLevel = 1;
	}

}

VitalityComponent::~VitalityComponent()
{

}

void VitalityComponent::update()
{

	if (m_hasFiniteLifetime == true) {
		_updateFiniteLifetime();
	}

	if (m_isRegenerative) {
		_updateRegeneration();
	}
}

void VitalityComponent::_levelUp()
{

	if (m_currentLevel < m_maxLevels) {

		m_currentLevel++;
		auto& level = m_regenLevels[(uint_fast64_t)m_currentLevel - 1];
		m_isBroken = false;
		parent()->setPhysicsActive(true);
		m_resistance = level.resistance;
		
		//Update the color based on the new level
		auto& renderComponent = parent()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
		renderComponent->setColor(level.color);
	}

	



}

void VitalityComponent::_updateFiniteLifetime()
{

	std::chrono::steady_clock::time_point now_time = std::chrono::steady_clock::now();
	std::chrono::duration<double, std::milli> timeDiffMilliSeconds = now_time - m_lifeTimeTimeSnapshot;

	this->m_lifetimeRemaining -= timeDiffMilliSeconds;

	if (this->m_lifetimeRemaining.count() <= 0)
	{

		//Mark this object for removal so that the removal loop will delete it
		parent()->setRemoveFromWorld(true);

	}
	else
	{
		m_lifeTimeTimeSnapshot = now_time;

		//If this particle should fade over time, then adjust its alpha value
		if (m_isLifetimeAlphaFade)
		{
			//Todo:move this to the render component and have it check the lifetime to adjust its alpha
			parent()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT)->setColorAlpha(int(255 * (m_lifetimeRemaining / m_lifetime)));
		}


	}

}

void VitalityComponent::_updateRegeneration()
{

	//If the broken flag was set in the contact listener, then set the physics active to false
	if (m_isBroken == true) {
		parent()->setPhysicsActive(false);
	}
	
	std::chrono::steady_clock::time_point now_time = std::chrono::steady_clock::now();
	std::chrono::duration<double, std::milli> timeDiffMilliSeconds = now_time - m_regenTimeSnapshot;

	//If this piece is broken and we have met the regen time, then level up the piece
	if (m_isBroken == true && timeDiffMilliSeconds.count() >= m_regenSpeed) {

		_levelUp();

	}

}


void VitalityComponent::inflictDamage(float damage)
{

	m_health -= damage;
	if (m_health <= 0) {

		if (m_isRegenerative == false) {
			m_isDestroyed = true;
		}
		else {
			m_isBroken = true;
			m_regenTimeSnapshot = std::chrono::steady_clock::now();
		}

	}

}

bool VitalityComponent::testResistance(float force)
{

	if (force >= m_resistance) {
		if (m_isRegenerative == false) {
			m_isDestroyed = true;
			parent()->setRemoveFromWorld(true);
		}
		else {

			m_isBroken = true;
			m_regenTimeSnapshot = std::chrono::steady_clock::now();
		}
		return false;
	}
	else {
		return true;
	}


}



