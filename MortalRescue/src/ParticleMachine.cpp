#include <Box2D/Box2D.h>

#include "ParticleMachine.h"
#include "ParticleEmission.h"
#include "game.h"



ParticleMachine::ParticleMachine()
{
}


ParticleMachine::~ParticleMachine()
{
}

void ParticleMachine::update()
{
	this->runParticleEmissions();

}

void ParticleMachine::runParticleEmissions()
{
	ParticleEmission* particleEmission = NULL;

	while (this->particleEmissions.empty() == false)
	{
		particleEmission = this->particleEmissions.back();

		this->emit(
			"PARTICLE1_POOL",
			particleEmission->originX,	// X position
			particleEmission->originY,	//Y Position
			particleEmission->forceMin,	//Force Min
			particleEmission->forceMax,	//force Max
			particleEmission->lifetimeMin,	//Lifetime Min
			particleEmission->lifetimeMax,	//Lifetime Max
			particleEmission->alphaFade,	// Alpha fade
			particleEmission->angleMin,	//Angle min
			particleEmission->angleMax,	//Angle Max
			particleEmission->particleSizeMin,	//Size Min
			particleEmission->particleSizeMax,	//Size Max
			particleEmission->colorRangeBegin,	//Color Min
			particleEmission->colorRangeEnd,	//Color Max
			particleEmission->particleCountMin,	//Particle count min
			particleEmission->particleCountMax	//Particle count max
		);

		this->particleEmissions.pop_back();
	}

}

void ParticleMachine::add(ParticleEmission* particleEmission)
{

	this->particleEmissions.push_back(particleEmission);



}


void ParticleMachine::emit(
	string poolId,
	float originX,
	float originY,
	int forceMin,
	int forceMax,
	float lifetimeMin,
	float lifetimeMax,
	bool alphaFade,
	float angleMin,
	float angleMax,
	float particleSizeMin,
	float particleSizeMax,
	SDL_Color colorRangeBegin,
	SDL_Color colorRangeEnd,
	int particleSpawnCountMin,
	int particleSpawnCountMax
)
{

	//Calculate the range of the angle in which the particles will be emitted
	float angleRange = angleMax - angleMin;
	float particleAngle;

	//If the particle count min and max are different, then generate a random count
	//otherwise just use the max
	int particleCount = 0;
	if (particleSpawnCountMin != particleSpawnCountMax)
	{
		particleCount = game->util.generateRandomNumber(particleSpawnCountMin, particleSpawnCountMax);
	}
	else
	{
		particleCount = particleSpawnCountMax;
	}

	//Emit each of the particles
	for (int i = 0; i < particleCount; i++)
	{

		//Get the particle object from the pre-populated particle pool
		ParticleObject* particle = game->objectPoolManager.get(poolId);

		//If the returned particle is null, then the pool has run out, so do nothing
		if (particle != NULL)
		{
			//Generate a random force
			int force = 0;
			if (forceMin != forceMax)
			{
				force = game->util.generateRandomNumber(forceMin, forceMax);
			}
			else
			{
				force = forceMax;
			}

			//Set lifetime alpha fade flag
			particle->isLifetimeAlphaFade = alphaFade;

			//Set the color of the particle. Randomize the color values if they are different
			SDL_Color color = { 255,255,255,255 };
			if (colorRangeBegin.r != colorRangeEnd.r)
			{
				color.r = game->util.generateRandomNumber(colorRangeBegin.r, colorRangeEnd.r);
			}
			if (colorRangeBegin.g != colorRangeEnd.g)
			{
				color.g = game->util.generateRandomNumber(colorRangeBegin.g, colorRangeEnd.g);
			}
			if (colorRangeBegin.b != colorRangeEnd.b)
			{
				color.b = game->util.generateRandomNumber(colorRangeBegin.b, colorRangeEnd.b);
			}
			particle->color = color;

			//Set the size of the particle. If zero is passed in then default to the particle size
			//in th eparticle definition
			float particleSize = 0;
			if (particleSizeMin != 0 && particleSizeMax != 0)
			{
				if (particleSizeMin != particleSizeMax)
				{
					particleSize = game->util.generateRandomNumber(particleSizeMin, particleSizeMax);
				}
				else
				{
					particleSize = particleSizeMax;
				}

				particle->xSize = particleSize * game->config.scaleFactor;
				particle->ySize = particleSize * game->config.scaleFactor;

			}

			//Set the particles lifetime in miliseconds. If a zero is passed in, then it will remain the value 
			//when it was built freom the pool definition
			float particleLifetime = 0;
			if (lifetimeMin != 0 and lifetimeMax != 0)
			{
				if (lifetimeMin != lifetimeMax)
				{
					particleLifetime = game->util.generateRandomNumber(lifetimeMin, lifetimeMax);
				}
				else
				{
					particleLifetime = lifetimeMax;
				}

				particle->lifetime = particle->lifetimeRemaining = std::chrono::duration<float>(particleLifetime);
			}


			//Calculate the emit angle/direction that the particle will travel in
			particleAngle = ((float)i / (float)particleCount) * angleRange;
			particleAngle = angleMin + particleAngle;
			particleAngle = particleAngle * DEGTORAD;

			//Calculate velocity vector
			float velocityX = cos(particleAngle) * force;
			float velocityY = sin(particleAngle) * force;
			b2Vec2 velocityVector = b2Vec2(velocityX, velocityY);

			//create the starting position vector of the particle
			b2Vec2 positionVector = b2Vec2(originX, originY);

			//Set both eh starting position and the velocity of th eparticle
			particle->physicsBody->SetTransform(positionVector, particleAngle);
			particle->physicsBody->SetLinearVelocity(velocityVector);
			//particle->physicsBody->SetBullet(true);

			//set animation state to active
			particle->currentAnimationState = "ACTIVE";

			//Add the particle to the game world
			game->addGameObject(particle, game->MAIN);
		}

	}
}

void ParticleMachine::fireBullet(
	string poolId,
	int originX,
	int originY,
	float angle,
	int force)
{

	SDL_Color defaultColor = {255,255,255,255};

	this->emit(
		poolId,
		originX,	// X position
		originY,	//Y Position
		force,	//Force Min
		force,	//force Max
		0,	//Lifetime Min
		0,	//Lifetime Max
		false,	// Alpha fade
		angle,	//Angle min
		angle,	//Angle Max
		0,	//Size Min
		0,	//Size Max
		defaultColor,	//Color Min
		defaultColor,	//Color Max
		1,	//Particle count min
		1	//Particle count max
	);




}

