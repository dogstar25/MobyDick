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
		m_hasInfiniteLifetime = true;
	}
	else {
		m_hasInfiniteLifetime = false;
	}

	m_lifetimeRemaining = std::chrono::duration<float>(m_lifetime);
	m_isLifetimeAlphaFade = vitalityComponentJSON["lifetimeAlphaFade"].asBool();

}

VitalityComponent::~VitalityComponent()
{

}

void VitalityComponent::update()
{

	if (m_hasInfiniteLifetime == false) {
		updateFiniteLifetime();
	}
}

void VitalityComponent::updateFiniteLifetime()
{

	std::chrono::steady_clock::time_point now_time = std::chrono::steady_clock::now();
	std::chrono::duration<double, std::milli> timeDiffMilliSeconds = now_time - m_timeSnapshot;

	this->m_lifetimeRemaining -= timeDiffMilliSeconds;

	if (this->m_lifetimeRemaining.count() <= 0)
	{

		//Mark this object for removal so that the removal loop will delete it
		parent()->setRemoveFromWorld(true);

	}
	else
	{
		m_timeSnapshot = now_time;

		//If this particle should fade over time, then adjust its alpha value
		if (m_isLifetimeAlphaFade)
		{
			//Todo:move this to the render component and have it check the lifetime to adjust its alpha
			parent()->getComponent<RenderComponent>()->setColorAlpha(int(255 * (m_lifetimeRemaining / m_lifetime)));
		}


	}

}




