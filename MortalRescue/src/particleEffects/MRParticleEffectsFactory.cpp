#include "MRParticleEffectsFactory.h"
#include "BaseConstants.h"
#include "particleEffects/ParticleEffectsFactory.h"


#include <iostream>


ParticleEffect MRParticleEffectsFactory::create(std::string particleType)
{
    ParticleEffect particleEffect;

    if (particleType == "ricochet") {

        particleEffect = ParticleEffects::ricochet;
    }
    else if (particleType == "deflect") {

        particleEffect = ParticleEffects::deflect;
    }
    else if (particleType == "scrap") {

        particleEffect = ParticleEffects::scrap;
    }
    else if (particleType == "spark") {

        particleEffect = ParticleEffects::spark;
    }
    else if (particleType == "impactSmoke") {

        particleEffect = ParticleEffects::impactSmoke;
    }

    else {
        particleEffect = ParticleEffectsFactory::create(particleType);
    }

    return particleEffect;
}
