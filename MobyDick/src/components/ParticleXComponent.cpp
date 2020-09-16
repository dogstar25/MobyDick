#include "ParticleXComponent.h"


#include "../game.h"


ParticleXComponent::ParticleXComponent(Json::Value definitionJSON )
{
	Json::Value particleComponentJSON = definitionJSON["particleXComponent"];


	////Add any effects that may be pre-built into the particle emitter
	//for (Json::Value itrEffect : particleComponentJSON["effects"])
	//{

	//	addParticleEffect(itrEffect);

	//}
	//for(auto effect : particleComponentJSON)

	//particleXComponent->addParticleEffect(ParticleEffects::ricochet);
	//particleXComponent->setType(ParticleEmitterType::CONTINUOUS);
	//particleXComponent->setEmissionInterval(std::chrono::duration<float>(0.002));

}

ParticleXComponent::~ParticleXComponent()
{

}

void ParticleXComponent::setEmissionInterval(std::chrono::duration<float> emissionInterval)
{
	m_emissionInterval = emissionInterval;
}


void ParticleXComponent::update()
{

	std::chrono::steady_clock::time_point now_time = std::chrono::steady_clock::now();
	std::chrono::duration<double> time_diff = now_time - m_timeSnapshot;

	//Only emit particles at the given time interval
	if (time_diff < m_emissionInterval) {
		return;
	}
	else{
		m_timeSnapshot = now_time;
	}

	for (auto& effect : m_particleEffects) {


		//If the particle count min and max are different, then generate a random count
		//that is between min and max , otherwise just use the max
		auto particleCount = util::generateRandomNumber(effect.particleSpawnCountMin, effect.particleSpawnCountMax);

		auto parentTransformComponent = parent()->transformComponent.value();

		for (int i = 0; i < particleCount; i++)
		{

			//Get the particle object from the pre-populated particle pool
			std::optional<std::shared_ptr<GameObject>> particle = ObjectPoolManager::instance().getPooledObject(effect.poolId);
			//auto& particle = std::make_shared<GameObject>("PARTICLE_SMOKE_1", -50.0f, -50.0f, 0.0f);
			//particle->init(false);

			//If the returned particle is null, then the pool has run out, so do nothing
			if (particle)
			{
				const auto& physicsComponent = particle.value()->physicsComponent.value();
				const auto& renderComponent = particle.value()->renderComponent.value();
				const auto& vitalityComponent = particle.value()->vitalityComponent.value();
				const auto& transformComponent = particle.value()->transformComponent.value();

				//Force
				auto force = util::generateRandomNumber(effect.forceMin, effect.forceMax);

				//Lifetime alpha fade
				vitalityComponent->setIsLifetimeAlphaFade(effect.alphaFade);

				//Set the color of the particle. Randomize the color values if they are different
				SDL_Color color = util::generateRandomColor(effect.colorRangeBegin, effect.colorRangeEnd);
				renderComponent->setColor(color);

				//Size
				auto particleSize = util::generateRandomNumber(effect.particleSizeMin, effect.particleSizeMax);
//				std::cout << "Size " << particleSize << "\n";
				transformComponent->setSize(particleSize, particleSize);

				//Set the particles lifetime in miliseconds.
				vitalityComponent->setTimeSnapshot(std::chrono::steady_clock::now());
				float particleLifetime = 0;
				particleLifetime = util::generateRandomNumber(effect.lifetimeMin, effect.lifetimeMax);
				vitalityComponent->setLifetime(std::chrono::duration<float>(particleLifetime));
				vitalityComponent->setLifetimeRemaining(std::chrono::duration<float>(particleLifetime));
				vitalityComponent->setHasInfiniteLifetime(false);

				//Calculate the emit angle/direction that the particle will travel in
				auto angleRange = effect.angleMax - effect.angleMin;
				auto particleAngle = ((float)i / (float)particleCount) * angleRange;
				particleAngle += effect.angleMin;
				particleAngle = util::degreesToRadians(particleAngle);

				//Calculate velocity vector
				auto velocityX = cos(particleAngle) * force;
				auto velocityY = sin(particleAngle) * force;
				auto velocityVector = b2Vec2(velocityX, velocityY);

				//Position - If zero was passed in then use the location of the gameObject
				//that this ParticlrComponent belongs to
				b2Vec2 positionVector = {};
				if (effect.originMin.Length() > 0 || effect.originMax.Length() > 0) {

					positionVector.x = util::generateRandomNumber(effect.originMin.x, effect.originMax.x);
					positionVector.y = util::generateRandomNumber(effect.originMin.y, effect.originMax.y);

				}
				else {

					positionVector = parentTransformComponent->position();
				}

				positionVector.x /= GameConfig::instance().scaleFactor();
				positionVector.y /= GameConfig::instance().scaleFactor();

				physicsComponent->setPhysicsBodyActive(true);
				physicsComponent->setTransform(positionVector, particleAngle);
				physicsComponent->setLinearVelocity(velocityVector);

				//Add the particle to the game world
				Game::instance().addGameObject(particle.value(), LAYER_MAIN);

			}
		}

	}

	//If this is a one-time emission, then mark this emitter object to be removed
	if (m_type == ParticleEmitterType::ONETIME) {
		parent()->setRemoveFromWorld(true);
	}

}


void ParticleXComponent::addParticleEffect(ParticleEffect particleEffect) 
{ 
	
	m_particleEffects.push_back(particleEffect); 

}


