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
    else if (particleType == "jetpack") {

        particleEffect = ParticleEffects::jetpack;
    }
    else if (particleType == "jetpack_sparks") {

        particleEffect = ParticleEffects::jetpack_sparks;
    }
	else if (particleType == "explosionSmoke") {

		particleEffect = ParticleEffects::explosionSmoke;
	}
	else if (particleType == "turretScrap") {

		particleEffect = ParticleEffects::turretScrap;
	}
	else if (particleType == "meteor") {

		particleEffect = ParticleEffects::meteor;
	}
    else if (particleType == "radiation") {

        particleEffect = ParticleEffects::radiation;
    }
    

    else {
        particleEffect = ParticleEffectsFactory::create(particleType);
    }

    return particleEffect;
}
