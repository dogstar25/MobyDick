#pragma once

#include "BaseParticleEffects.h"

#include <memory>
#include <optional>


class ParticleEffectsFactory {



public:

	virtual ParticleEffect create(std::string particleType);


private:




};
