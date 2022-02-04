#include "ParticleEffectsFactory.h"


#include <iostream>


ParticleEffect ParticleEffectsFactory::create(std::string particleType)
{
    ParticleEffect particleEffect;

    if (particleType == "smoke") {

        particleEffect = ParticleEffects::smoke;
    }

    else {
        std::cout << "No ParticleEffect was Matched" << std::endl;
        particleEffect = ParticleEffects::smoke;
    }

    return particleEffect;
}
