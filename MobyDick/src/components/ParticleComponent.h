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
#include "../BaseConstants.h"
#include "../particleEffects/BaseParticleEffects.h"
#include "../Timer.h"

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
	bool alphaFade{};
	Timer lifetimeTimer{};

};


class ParticleComponent : public Component
{
public:
	ParticleComponent(Json::Value definitionJSON);
	~ParticleComponent();

	void update() override;
	void render();
	void setType(int type) { m_type = type; }
	void addParticleEffect(ParticleEffect particleEffect);
	std::optional<Particle*> getAvailableParticle();

private:

	int m_type{ParticleEmitterType::ONETIME};
	bool m_oneTimeEmitted{ false };
	std::vector<ParticleEffect> m_particleEffects;
	std::chrono::duration<float, std::milli> m_emissionInterval{ std::chrono::duration<float>(0.f) };
	std::vector<Particle> m_particles;

	Timer m_emissionTimer{};



};






#endif

