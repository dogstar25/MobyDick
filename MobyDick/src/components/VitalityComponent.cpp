#include "VitalityComponent.h"

#include "../GameObject.h"
#include "../ColorMap.h"



VitalityComponent::VitalityComponent()
{

}


VitalityComponent::VitalityComponent(Json::Value componentJSON)
{

	m_componentType = ComponentTypes::VITALITY_COMPONENT;

	m_speed = componentJSON["speed"].asFloat();
	m_rotationSpeed = componentJSON["rotationSpeed"].asFloat();
	//m_lifetime  = std::chrono::duration<float>(componentJSON["lifetime"].asFloat());

	m_health = m_maxHealth = componentJSON["health"].asFloat();

	float m_lifetime = m_OriginalLifetime =  componentJSON["lifetime"].asFloat();
	m_lifetimeTimer = Timer(m_lifetime);

	m_isLifetimeAlphaFade = componentJSON["lifetimeAlphaFade"].asBool();
	m_resistance = componentJSON["resistance"].asFloat();

	//Regeneration related
	if (componentJSON.isMember("regenerating")) {
		m_isRegenerative = true;
		auto& regenJSON = componentJSON["regenerating"];

			m_regenSpeed = regenJSON["regenerateSpeed"].asFloat();
			m_resistance = regenJSON["resistance"].asFloat();
			m_hideWhenBroken = regenJSON["hideWhenBroken"].asBool();

			auto level = 0;
			for (Json::Value itrlevel : regenJSON["levels"]) {

				LevelDefinition levelItem = {};
				levelItem.levelNum = itrlevel["level"].asInt();
				levelItem.resistance = itrlevel["resistance"].asInt();

				if (itrlevel.isMember("color")) {
					levelItem.color = ColorMap::instance().toSDLColor(itrlevel["color"].asString());
					util::colorApplyAlpha(levelItem.color, 255);
				}
				else {
					levelItem.color = Colors::WHITE;
					util::colorApplyAlpha(levelItem.color, 255);
				}

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

void VitalityComponent::render()
{
}

void VitalityComponent::update()
{

	if (m_lifetimeTimer.infiniteLifetime() == false) {
		_updateFiniteLifetime();
	}

	if (m_isRegenerative) {
		_updateRegeneration();
	}


}

void VitalityComponent::_levelUp()
{

	m_currentLevel++;
	auto& level = m_regenLevels[(uint_fast64_t)m_currentLevel - 1];
	parent()->enableCollision();
	parent()->enableRender();
	m_resistance = level.resistance;
		
	//Update the color based on the new level
	const auto& renderComponent = parent()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	renderComponent->setColor(level.color);

}

void VitalityComponent::_updateFiniteLifetime()
{

	if (m_lifetimeTimer.hasMetTargetDuration())
	{

		//Mark this object for removal so that the removal loop will delete it
		parent()->setRemoveFromWorld(true);

	}
	else
	{

		//If this particle should fade over time, then adjust its alpha value
		if (m_isLifetimeAlphaFade)
		{
			//Todo:move this to the render component and have it check the lifetime to adjust its alpha
			parent()->getComponent<RenderComponent>(
				ComponentTypes::RENDER_COMPONENT)->setColorAlpha(int(255 * (m_lifetimeTimer.percentTargetMet())));
		}

	}

}

void VitalityComponent::_updateRegeneration()
{

	//If this gameObject is considered broken and we have met the regen time, then regenerate and level up if applicable

	// we need a "broken" flag to represent what it means to be broken for
	//the vitality component.
	// physics



	if (m_isBroken == true && m_regenTimer.hasMetTargetDuration()) {

		if (m_currentLevel < m_maxLevels) {

			//Restore the object and if we aslo hid it while it was brokwen then unhide it
			m_isBroken = false;
			_restore();
			_levelUp();
		}
	}

}

void VitalityComponent::_breaK()
{
	m_isBroken = true;
	parent()->disableCollision();
	if (m_hideWhenBroken == true) {
		parent()->disableRender();
	}

}

void VitalityComponent::_restore()
{
	parent()->enableCollision();
	if (m_hideWhenBroken == true) {
		parent()->enableRender();
	}

}

bool VitalityComponent::inflictDamage(float damage)
{
	bool dead = false;

	m_health -= damage;
	if (m_health <= 0) {
		dead = true;
	}

	return dead;

}

bool VitalityComponent::addHealth(float health)
{

	if (m_health + health <= m_maxHealth) {

		m_health += health;
		return true;
	}

	return false;
}

bool VitalityComponent::testResistance(float force)
{

	if (force >= m_resistance) {
		if (m_isRegenerative == false) {
			parent()->setRemoveFromWorld(true);
		}
		else {

			_breaK();
			m_regenTimer = Timer(m_regenSpeed);
		}
		return false;
	}
	else {
		return true;
	}

}

void VitalityComponent::setLifetimeTimer(float lifetime)
{

	m_lifetimeTimer = Timer(lifetime);

}

void VitalityComponent::resetLifetime()
{

	m_lifetimeTimer.reset();

}

void VitalityComponent::reset()
{

	m_lifetimeTimer = Timer(m_OriginalLifetime);
	resetLifetime();

}
