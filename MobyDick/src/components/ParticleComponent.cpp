#include "ParticleComponent.h"

#include <iostream>
#include <algorithm>

#include "../game.h"
#include "../GameConfig.h"
#include "../Camera.h"
#include "../DebugPanel.h"
#include "../Clock.h"
#include "../particleEffects/ParticleEffectsMap.h"
#include "../EnumMaps.h"

extern std::unique_ptr<Game> game;

ParticleComponent::ParticleComponent(Json::Value definitionJSON )
{
	Json::Value particleComponentJSON = definitionJSON["particleComponent"];


	for (int i = 0; i < 300; i++) {

		m_particles.emplace_back(Particle());

	}

	auto emissionInterval = particleComponentJSON["emissionInterval"].asFloat();
	m_emissionTimer = Timer(emissionInterval);

	//Add any effects that may be pre-built into the particle emitter
	for (Json::Value itrEffect : particleComponentJSON["effects"])
	{
		auto effectId = itrEffect.asString();

		ParticleEffect effect = ParticleEffectsMap::instance().getParticleEffect(effectId);
		addParticleEffect(effect);
	}

	if (particleComponentJSON.isMember("type")) {
		m_type = EnumMap::instance().toEnum(particleComponentJSON["type"].asString());
	}

}

ParticleComponent::~ParticleComponent()
{

}

void ParticleComponent::render()
{

	for (auto& particle : m_particles) {

		if (particle.isActive == true) {

			SDL_FRect destRect =
			{ particle.position.x,
				particle.position.y,
				particle.size,
				particle.size
			};

			//Adjust destination for camera position
			destRect.x -= Camera::instance().frame().x;
			destRect.y -= Camera::instance().frame().y;

			//SDL_SetTextureColorMod(particle.texture, particle.color.r, particle.color.g, particle.color.b);
			//SDL_SetTextureAlphaMod(particle.texture, particle.color.a);
			//SDL_SetTextureBlendMode(particle.texture, SDL_BLENDMODE_ADD);

			game->renderer()->drawSprite(destRect, particle.color, particle.texture, nullptr, 0, false, SDL_Color{});

			//SDL_RenderCopyExF(
			//	RendererSDL::instance().renderer(),
			//	particle.texture,
			//	nullptr,
			//	&destRect,
			//	0,
			//	NULL,
			//	SDL_FLIP_NONE);
		}
	}

}


//void ParticleComponent::setEmissionInterval(std::chrono::duration<float> emissionInterval)
//{
//	m_emissionInterval = emissionInterval;
//}


void ParticleComponent::update()
{
	bool activeParticleFound = false;

	//First update the position,lifetime,etc of all active particles
	for (auto& particle : m_particles) {

		if (particle.isActive == true) {

			activeParticleFound = true;

			if (particle.lifetimeTimer.hasMetTargetDuration()) {

				particle.isAvailable = true;
				particle.isActive = false;
			}
			else {
			}

			//particle.position.x += particle.velocity.x * .01666 ;
			//particle.position.y += particle.velocity.y * .01666;
			float timeFactor{ GameConfig::instance().gameLoopStep() };
			if (SceneManager::instance().gameTimer().timeRemaining().count() > 0) {
				timeFactor = SceneManager::instance().gameTimer().timeRemaining().count();
			}
			particle.position.x += particle.velocity.x * timeFactor;
			particle.position.y += particle.velocity.y * timeFactor;

			if (particle.alphaFade == true) {
				Uint8 alpha = int(255 * particle.lifetimeTimer.percentTargetMet());
				particle.color.a = alpha;
			}
		}

	}

	//Now emit more particles IF,
	//The emission interval timer has been met AND
	//this is either a continuous emitter or a onetime emitter that hasnt fired yet
	if ((m_type == ParticleEmitterType::CONTINUOUS && m_emissionTimer.hasMetTargetDuration() == true) ||
		(m_type == ParticleEmitterType::ONETIME && m_oneTimeEmitted == false )) {

		for (auto& effect : m_particleEffects) {

			//Get the texture by retrieving the effects pooled object and grabbing its texture
			Texture* texture = parent()->parentScene()->objectPoolManager().getPoolObjectTexture(effect.poolId).get();

			//If the particle count min and max are different, then generate a random count
			//that is between min and max , otherwise just use the max
			auto particleCount = util::generateRandomNumber(effect.particleSpawnCountMin, effect.particleSpawnCountMax);

			auto parentTransformComponent = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

			for (int i = 0; i < particleCount; i++)
			{

				const auto& particle = getAvailableParticle();

				if (particle) {

					m_oneTimeEmitted = true;
					activeParticleFound = true;

					particle.value()->isAvailable = false;
					particle.value()->isActive = true;

					//Set the texture
					particle.value()->texture = texture;

					//Alpha Fade
					particle.value()->alphaFade = effect.alphaFade;

					//Set the color of the particle. Randomize the color values if they are different
					particle.value()->color = util::generateRandomColor(effect.colorRangeBegin, effect.colorRangeEnd);

					//Size
					particle.value()->size = util::generateRandomNumber(effect.particleSizeMin, effect.particleSizeMax);

					//Set the particles lifetime
					auto particleLifetime = util::generateRandomNumber(effect.lifetimeMin, effect.lifetimeMax);
					particle.value()->lifetimeTimer = Timer(particleLifetime);

					//Calculate the emit angle/direction that the particle will travel in
					//If this is a onetime emission, make each particle's angle symetric with the whole.
					//Otherwise, make each one random, but still within the angle range
					float emitAngle = 0;
					if (m_type == ParticleEmitterType::ONETIME) {
						auto angleRange = effect.angleMax - effect.angleMin;
						emitAngle = ((float)i / (float)particleCount) * angleRange;
					}
					else {
						emitAngle = util::generateRandomNumber(effect.angleMin, effect.angleMax);
					}

					//emitAngle += effect.angleMin;
					emitAngle = util::degreesToRadians(emitAngle);

					//Calculate velocity vector
					auto force = util::generateRandomNumber(effect.forceMin, effect.forceMax);

					//Adjust force by a factor so that is closer matches what box2d does and we can use the same
					//particle effect objects easier
					particle.value()->velocity.x = cos(emitAngle) * (force * PARTICLE_EMITTER_FORCE_ADJ);
					particle.value()->velocity.y = sin(emitAngle) * (force * PARTICLE_EMITTER_FORCE_ADJ);

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

					particle.value()->position.x = positionVector.x;
					particle.value()->position.y = positionVector.y;

				}

			}

		}
	}

	//If this is a one-time emission and all particles that were emitted are now inactive, 
	// then mark this emitter object to be removed
	if (m_type == ParticleEmitterType::ONETIME && activeParticleFound == false) {
		parent()->setRemoveFromWorld(true);
	}

}


void ParticleComponent::addParticleEffect(ParticleEffect particleEffect) 
{ 
	
	m_particleEffects.push_back(particleEffect); 

}

std::optional<Particle*> ParticleComponent::getAvailableParticle()
{
	std::optional<Particle*> availableParticle = std::nullopt;

	for (auto& particle : m_particles) {

		if (particle.isAvailable) {

			availableParticle = &particle;
			break;
		}

	}

	return availableParticle;
}


