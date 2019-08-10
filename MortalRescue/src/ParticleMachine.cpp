#include <Box2D/Box2D.h>

#include "ParticleMachine.h"
#include "Explosion.h"
#include "game.h"



ParticleMachine::ParticleMachine()
{
}


ParticleMachine::~ParticleMachine()
{
}

void ParticleMachine::update()
{
	this->runExplosions();

}

void ParticleMachine::runExplosions()
{
	Explosion* explosion=NULL;

	while (this->explosions.empty() == false)
	{
		explosion = this->explosions.back();
		float angleRange = explosion->angleMax - explosion->angleMin;
		float particleAngle;
		for (int i = 0; i < explosion->particleCount; i++)
		{
			ParticleObject* particle = game->objectPoolManager.get(explosion->particleId);

			if (particle != NULL)
			{
				//float angle = (i / explosion->particleCount) * angleRange;
				particleAngle = ((float)i / (float)explosion->particleCount) * angleRange;

				particleAngle = explosion->angleMin + particleAngle;
				particleAngle = particleAngle * DEGTORAD;

				float velocityX = 0;
				float velocityY = 0;
				if (explosion->forceMax != explosion->forceMin) 
				{
					velocityX = cos(particleAngle) * game->util.generateRandomNumber(explosion->forceMin, explosion->forceMax);
					velocityY = sin(particleAngle) * game->util.generateRandomNumber(explosion->forceMin, explosion->forceMax);
				}
				else
				{
					velocityX = cos(particleAngle) * explosion->forceMax;
					velocityY = sin(particleAngle) * explosion->forceMax;
				}

				b2Vec2 positionVector = b2Vec2(explosion->originX, explosion->originY);
				b2Vec2 velocityVector = b2Vec2(velocityX, velocityY);

				particle->physicsBody->SetTransform(positionVector, 0);
				particle->physicsBody->SetLinearVelocity(velocityVector);
				//particle->physicsBody->SetBullet(true);
				//particle->physicsBody->ser
				particle->currentAnimationState = "ACTIVE";


				//todo
				//SDL_SetTextureAlphaMod
				//SDL_SetTextureColorMod




				game->addGameObject(particle, game->MAIN);
			}
			//game->debugPanel->addItem("NEW_EXPLOSION_PARTICLE", to_string(true));

		}


		this->explosions.pop_back();
	}

	/*
	for (auto & explosion : this->explosions)
	{
		for (int i=0;i<explosion->particleCount;i++)
		{
			game->debugPanel->addItem("NEW_EXPLOSION_PARTICLE", to_string(true));
		}

	}
	*/
}

void ParticleMachine::execute( Explosion* explosion)
{

	



}

void ParticleMachine::add(Explosion* explosion)
{

	this->explosions.push_back(explosion);



}
