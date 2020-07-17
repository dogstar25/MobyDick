#include "ParticleComponent.h"

#include "../GameObjectManager.h"
#include "../GameObject.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"


ParticleComponent::ParticleComponent(Json::Value definitionJSON )
{
	Json::Value particleComponentJSON = definitionJSON["particleComponent"];

	m_gameObjectId = definitionJSON["id"].asString();;

	//initialize stuff
	//All particle attributes get set when a particle is spawned from the particle machine
	m_isAvailable = true;
	m_isLifetimeAlphaFade = false;
	m_lifetimeRemaining = std::chrono::duration<float>(0);
	m_lifetime = std::chrono::duration<float>(0);
	m_isActive = false;
	m_hasInfiniteLifetime = true;

}

ParticleComponent::~ParticleComponent()
{

}

void ParticleComponent::update(std::shared_ptr<GameObject>gameObject)
{

	std::chrono::steady_clock::time_point now_time = std::chrono::steady_clock::now();
	std::chrono::duration<double, std::milli> timeDiffMilliSeconds = now_time - m_time_snapshot;

	if (m_hasInfiniteLifetime == false)
	{
		this->m_lifetimeRemaining -= timeDiffMilliSeconds;

		if (this->m_lifetimeRemaining.count() <= 0)
		{

			//Mark this object for removal so that the removal loop will delete it
			gameObject->setRemoveFromWorld(true);


		}
		else
		{
			m_time_snapshot = now_time;

			//If this particle should fade over time, then adjust its alpha value
			if (m_isLifetimeAlphaFade)
			{
				gameObject->getComponent<RenderComponent>()->setColorAlpha(int(255 * (m_lifetimeRemaining / m_lifetime)));
			}


		}
	}
}

void ParticleComponent::reset()
{

	m_lifetimeRemaining = m_lifetime;
	setAvailable(true);

}





