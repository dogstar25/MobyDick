#include "HeroFireAction.h"
#include <iostream>
#include <box2d/box2d.h>
#include <SDL2/SDL.h>
#include "../ParticleEmission.h"
#include "../ParticleMachine.h"
#include "../GameObject.h"
#include "../Scene.h"


HeroFireAction::HeroFireAction()
{

}

HeroFireAction::~HeroFireAction()
{


}

void HeroFireAction::perform(GameObject* gameObject)
{



	//gameObject->scene()->particleMachine().fireBullet("BULLET1_POOL",
	//	gameObject->getComponent<TransformComponent>()->position(),
	//	gameObject->getComponent<TransformComponent>()->angle(),
	//	40);


	////use the collision point for the particle emission
	//float x = 4;
	//float y = 4;
	//b2Vec2 particleOrigin = { x,y };

	////temp color code
	//SDL_Color colorMin = { 0,0,0,255 };
	//SDL_Color colorMax = { 225,255,255,255 };


	//ParticleEmission* particleEmission = new ParticleEmission(
	//	"SMOKE1_POOL",
	//	particleOrigin, //min position
	//	particleOrigin,	//max position
	//	5,	//Force Min
	//	15,	//force Max
	//	1.55,	//Lifetime Min
	//	1.55,	//Lifetime Max
	//	true,	// Alpha fade
	//	0,	//Angle min
	//	360,	//Angle Max
	//	12,	//Size Min
	//	32,	//Size Max
	//	colorMin,	//Color Min
	//	colorMax,	//Color Max
	//	30,	//Particle count min
	//	60	//Particle count max
	//);
	//ParticleMachine::instance().add(particleEmission);

}
