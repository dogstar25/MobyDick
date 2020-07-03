#ifndef PARTICLE_COMPONENT_H
#define PARTICLE_COMPONENT_H

#include <string>

#include "Component.h"

class ParticleComponent : public Component
{
public:
	ParticleComponent();
	ParticleComponent(std::string gameObjectId);
	~ParticleComponent();

	void update() override;


private:
	bool isAvailable;
	std::string poolId;

};

#endif

