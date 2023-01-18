#include "BBParticleEffectsFactory.h"
#include "BaseConstants.h"
#include "particleEffects/ParticleEffectsFactory.h"


#include <iostream>


ParticleEffect BBParticleEffectsFactory::create(std::string particleType)
{
    ParticleEffect particleEffect;

    if (particleType == "spark") {

        particleEffect = ParticleEffects::spark;
    }
    

    else {
        particleEffect = ParticleEffectsFactory::create(particleType);
    }

    return particleEffect;
}
