#ifndef PARTICLE_COMPONENT_H
#define PARTICLE_COMPONENT_H

#include <string>

#include "Component.h"

class ParticleComponent : public Component
{
public:
	ParticleComponent();
	ParticleComponent(const GameObject& gameObject);
	~ParticleComponent();

	void update() override;


private:
	bool isAvailable;
	std::string poolId;

};

#endif

