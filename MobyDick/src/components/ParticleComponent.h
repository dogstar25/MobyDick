#ifndef PARTICLE_COMPONENT_H
#define PARTICLE_COMPONENT_H

#include <string>
#include <memory>
#include <chrono>
#include <vector>
#include <optional>

#include <json/json.h>

#include "Component.h"
#include "../Globals.h"
#include "../config_data/ParticleEffect.h"

class GameObject;

struct Particle
{
	SDL_FPoint position{ 0,0 };
	float size{ 0 };
	float speed { 1 };
	SDL_FPoint velocity{ 0 };
	bool isAvailable { true };	
	bool isActive{ false };
	SDL_Texture* texture;
	SDL_Color color{ 255,255,255,255 };

	std::chrono::steady_clock::time_point timeSnapshot{ std::chrono::steady_clock::now() };
	std::chrono::duration<float, std::milli> lifetime;
	std::chrono::duration<float, std::milli> lifetimeRemaining;
};


class ParticleComponent : public Component
{
public:
	ParticleComponent(Json::Value definitionJSON);
	~ParticleComponent();

	void update() override;
	void render();
	void init();
	void setType(int type) { m_type = type; }
	void setEmissionInterval(std::chrono::duration<float>);
	void addParticleEffect(ParticleEffect particleEffect);
	std::optional<Particle*> getAvailableParticle();

private:

	int m_type{ParticleEmitterType::ONETIME};
	bool m_oneTimeEmitted{ false };
	std::vector<ParticleEffect> m_particleEffects;
	std::chrono::duration<float, std::milli> m_emissionInterval{ std::chrono::duration<float>(0.f) };
	std::chrono::steady_clock::time_point m_timeSnapshot{ std::chrono::steady_clock::now() };
	std::chrono::steady_clock::time_point m_EmissiontimeSnapshot{ std::chrono::steady_clock::now() };
	std::vector<Particle> m_particles;



};






#endif

