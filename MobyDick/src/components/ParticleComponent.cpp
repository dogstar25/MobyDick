#include "ParticleComponent.h"


#include "../game.h"
#include "../GameConfig.h"
#include "../Camera.h"


ParticleComponent::ParticleComponent(Json::Value definitionJSON )
{
	Json::Value particleComponentJSON = definitionJSON["particleComponent"];


	for (int i = 0; i < 10000; i++) {

		m_particles.emplace_back(Particle());

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

			SDL_SetTextureColorMod(particle.texture, particle.color.r, particle.color.g, particle.color.b);
			SDL_SetTextureAlphaMod(particle.texture, particle.color.a);
			SDL_SetTextureBlendMode(particle.texture, SDL_BLENDMODE_ADD);

			SDL_RenderCopyExF(
				Renderer::instance().SDLRenderer(),
				particle.texture,
				nullptr,
				&destRect,
				0,
				NULL,
				SDL_FLIP_NONE);
		}
	}

}


void ParticleComponent::setEmissionInterval(std::chrono::duration<float> emissionInterval)
{
	m_emissionInterval = emissionInterval;
}


void ParticleComponent::update()
{
	bool activeParticleFound = false;

	//First update the position,lifetime,etc of all active particles
	for (auto& particle : m_particles) {

		if (particle.isActive == true) {

			activeParticleFound = true;

			std::chrono::steady_clock::time_point nowTime = std::chrono::steady_clock::now();
			auto timeDiff = nowTime - particle.timeSnapshot;
			particle.lifetimeRemaining -= timeDiff;
			if (particle.lifetimeRemaining.count() <= 0) {

				particle.isAvailable = true;
				particle.isActive = false;

			}

			//particle.position.x += particle.velocity.x * .1666 ;
			//particle.position.y += particle.velocity.y * .1666;
			auto milliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(timeDiff);
			float test = milliSeconds.count();
			particle.position.x += particle.velocity.x * float(milliSeconds.count() *.001);
			particle.position.y += particle.velocity.y * float(milliSeconds.count() * .001);

		}

		//add alpha fade
		Uint8 alpha = 255;
		alpha = int(255 * (particle.lifetimeRemaining.count() / particle.lifetime.count()));
		particle.color.a = alpha;

	}

	//Only emit particles at the given time interval
	std::chrono::steady_clock::time_point now_time = std::chrono::steady_clock::now();
	std::chrono::duration<double> time_diff = now_time - m_timeSnapshot;

	if (time_diff < m_emissionInterval) {
		return;
	}
	else {
		m_timeSnapshot = now_time;
	}

	//Now emit more particles
	for (auto& effect : m_particleEffects) {

		//Get the texture by retrieving the effects pooled object and grabbing its texture
		SDL_Texture* texture = ObjectPoolManager::instance().getPoolObjectTexture(effect.poolId);

		//If the particle count min and max are different, then generate a random count
		//that is between min and max , otherwise just use the max
		auto particleCount = util::generateRandomNumber(effect.particleSpawnCountMin, effect.particleSpawnCountMax);

		auto parentTransformComponent = parent()->getComponent<TransformComponent>();

		for (int i = 0; i < particleCount; i++)
		{

			auto& particle = getAvailableParticle();

			if (particle) {

				particle.value()->isAvailable = false;
				particle.value()->isActive = true;

				//Set the texture
				particle.value()->texture = texture;

				//Set the color of the particle. Randomize the color values if they are different
				particle.value()->color = util::generateRandomColor(effect.colorRangeBegin, effect.colorRangeEnd);

				//Size
				particle.value()->size = util::generateRandomNumber(effect.particleSizeMin, effect.particleSizeMax);

				//Set the particles lifetime in miliseconds.
				//std::chrono::steady_clock::time_point nowTime = std::chrono::steady_clock::now();
				auto particleLifetime = util::generateRandomNumber(effect.lifetimeMin, effect.lifetimeMax);
				particle.value()->timeSnapshot = now_time;
				particle.value()->lifetime = (std::chrono::duration<float>(particleLifetime));
				particle.value()->lifetimeRemaining = (std::chrono::duration<float>(particleLifetime));

				//Calculate the emit angle/direction that the particle will travel in
				auto angleRange = effect.angleMax - effect.angleMin;
				auto emitAngle = ((float)i / (float)particleCount) * angleRange;
				emitAngle += effect.angleMin;
				emitAngle = util::degreesToRadians(emitAngle);

				//Calculate velocity vector
				auto force = util::generateRandomNumber(effect.forceMin, effect.forceMax);
				particle.value()->velocity.x = cos(emitAngle) * force;
				particle.value()->velocity.y = sin(emitAngle) * force;

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

				//positionVector.x /= GameConfig::instance().scaleFactor();
				//positionVector.y /= GameConfig::instance().scaleFactor();

				particle.value()->position.x = positionVector.x;
				particle.value()->position.y = positionVector.y;

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


