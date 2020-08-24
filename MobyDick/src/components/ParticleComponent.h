#ifndef PARTICLE_COMPONENT_H
#define PARTICLE_COMPONENT_H

#include <string>
#include <memory>
#include <chrono>
#include <vector>

#include <json/json.h>

#include "Component.h"
#include "../ParticleEmission.h"
#include "../config_data/ParticleEffect.h"

class GameObject;

class ParticleComponent : public Component
{
public:
	ParticleComponent(Json::Value definitionJSON);
	~ParticleComponent();

	void update() override;
	void setType(int type) { m_type = type; }
	void addParticleEffect(ParticleEffect particleEffect);

private:
	int m_type;
	std::vector<ParticleEffect> m_particleEffects;


};






#endif

