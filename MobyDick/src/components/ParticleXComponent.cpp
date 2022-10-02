#include "ParticleXComponent.h"


#include "../EnumMap.h"
#include "../game.h"

extern std::unique_ptr<Game> game;

ParticleXComponent::ParticleXComponent(Json::Value componentJSON)
{
	m_componentType = ComponentTypes::PARTICLE_X_COMPONENT;

	if (componentJSON.isMember("type")) {
		m_type = game->enumMap()->toEnum(componentJSON["type"].asString());
	}
	if (componentJSON.isMember("emissionLayer")) {
		m_emissionLayer = game->enumMap()->toEnum(componentJSON["emissionLayer"].asString());
	}

	float emissionInterval = componentJSON["emissionInterval"].asFloat();
	m_emissionInterval = (std::chrono::duration<float>(emissionInterval));
	m_emissionTimer = Timer(emissionInterval, true);

	//Add any effects that may be pre-built into the particle emitter
	for (Json::Value itrEffect : componentJSON["effects"])
	{

		auto effectId = itrEffect.asString();

		//ParticleEffect effect = ParticleEffectsMap::instance().getParticleEffect(effectId);
		ParticleEffect effect = game->particleEffectsFactory()->create(effectId);
		addParticleEffect(effect);
	
	}
}

ParticleXComponent::~ParticleXComponent()
{

}

void ParticleXComponent::setEmissionInterval(float emissionInterval)
{
	m_emissionInterval = std::chrono::duration<float>(emissionInterval);
}


void ParticleXComponent::update()
{

	if(m_emissionTimer.hasMetTargetDuration() == true || m_type == ParticleEmitterType::ONETIME){

		for (auto& effect : m_particleEffects) 
		{

			//If the particle count min and max are different, then generate a random count
			//that is between min and max , otherwise just use the max
			auto particleCount = util::generateRandomNumber(effect.particleSpawnCountMin, effect.particleSpawnCountMax);
			auto parentTransformComponent = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

			for (int i = 0; i < particleCount; i++) 
			{
				//Get the particle object from the pre-populated particle pool
				std::optional<std::shared_ptr<GameObject>> particle = 
					parent()->parentScene()->objectPoolManager().getPooledObject(effect.poolId);

				//If the returned particle is null, then the pool has run out, so do nothing
				if (particle)
				{
					const auto& physicsComponent = particle.value()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
					const auto& renderComponent = particle.value()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
					const auto& vitalityComponent = particle.value()->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);
					const auto& transformComponent = particle.value()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

					//Force
					auto force = util::generateRandomNumber(effect.forceMin, effect.forceMax);

					//Lifetime alpha fade
					vitalityComponent->setIsLifetimeAlphaFade(effect.alphaFade);

					//Set the color of the particle. Randomize the color values if they are different
					SDL_Color color = util::generateRandomColor(effect.colorRangeBegin, effect.colorRangeEnd);
					renderComponent->setColor(color);

					//Size
					auto particleSize = util::generateRandomNumber(effect.particleSizeMin, effect.particleSizeMax);
					transformComponent->setSize(particleSize, particleSize);

					//Set the particles lifetime in miliseconds. If zero lifetime then it has infinite lifetime
					float particleLifetime = util::generateRandomNumber(effect.lifetimeMin, effect.lifetimeMax);
					vitalityComponent->setLifetimeTimer(particleLifetime);

					//Calculate the emit angle/direction that the particle will travel in
					//If this is a onetime emission, make each particles angle symetric with the whole
					//otherwise, make each one random, but still within the angle range
					//float emitterAngle = util::radiansToDegrees(parent()->getAngle());
					float emitterAngle = util::radiansToDegrees(parent()->getAngle());
					float particleAngle{};


					if (m_type == ParticleEmitterType::ONETIME) 
					{
						auto angleRange = effect.angleMax - effect.angleMin;
						particleAngle = emitterAngle + ((float)i / (float)particleCount) * angleRange;
					}
					else 
					{
						particleAngle = emitterAngle + util::generateRandomNumber(effect.angleMin, effect.angleMax);
					}

					//particleAngle += effect.angleMin;
					particleAngle = util::degreesToRadians(particleAngle);

					//Calculate velocity vector
					auto velocityX = cos(particleAngle) * force;
					auto velocityY = sin(particleAngle) * force;
					auto velocityVector = b2Vec2(velocityX, velocityY);

					//Position - If zero was passed in then use the location of the gameObject
					//that this ParticlrComponent belongs to
					b2Vec2 positionVector = {};
					if (effect.originMin.Length() > 0 || effect.originMax.Length() > 0) 
					{
						positionVector.x = util::generateRandomNumber(effect.originMin.x, effect.originMax.x);
						positionVector.y = util::generateRandomNumber(effect.originMin.y, effect.originMax.y);
					}
					else 
					{
						positionVector = parentTransformComponent->position();
					}

					positionVector.x /= GameConfig::instance().scaleFactor();
					positionVector.y /= GameConfig::instance().scaleFactor();

					physicsComponent->setPhysicsBodyActive(true);
					physicsComponent->setTransform(positionVector, particleAngle);
					physicsComponent->setLinearVelocity(velocityVector);

					//Add the particle to the game world
					//ToDo:Make Layer configurable
					parent()->parentScene()->addGameObject(particle.value(), m_emissionLayer);

				}
				else {
					//std::cout << "No Particle available\n";
				}
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


