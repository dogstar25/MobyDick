#include "ParticleObject.h"
#include "game.h"
#include <string>
#include <json/json.h>

using namespace std::chrono_literals;
using namespace std;

ParticleObject::ParticleObject()
{
}

ParticleObject::ParticleObject(string gameObjectId, float xMapPos, float yMapPos, float angleAdjust) :
	WorldObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{
	//initialize stuff
	this->isAvailable = false;
	this->isLifetimeAlphaFade = false;
	this->lifetimeRemaining = this->lifetime = std::chrono::duration<float>(0);


}

ParticleObject::~ParticleObject()
{
}


void ParticleObject::update()
{
	GameObject::update();
	steady_clock::time_point now_time = steady_clock::now();

	duration<double, milli> timeDiffMilliSeconds = now_time - this->time_snapshot;

	//debug
	//game->debugPanel->addItem("LIFETIME", to_string(this->lifetime));
	//game->debugPanel->addItem("PART_TIMEDIFF_MILI", to_string(timeDiffMilliSeconds.count()));
	//game->debugPanel->addItem("PART_TIMEDIFF_SEC", to_string(timeDiffSeconds.count()));

	this->lifetimeRemaining -= timeDiffMilliSeconds;

	if (this->lifetimeRemaining.count() <= 0)
	{

		//reset the particle object
		//game->objectPoolManager.reset(this);
		//game->debugPanel->addItem("RESET_PARTICLE", "TRUE");

		//Mark this object for removal so that the removal loop will delete it
		this->removeFromWorld = true;

	}
	else
	{
		this->time_snapshot = now_time;
		//game->debugPanel->addItem("RESET_PARTICLE", "FALSE");

		//If this particle should fade over time, then adjust its alpha value
		if (this->isLifetimeAlphaFade)
		{
			this->color.a = 255 * ( this->lifetimeRemaining / this->lifetime);
		}


	}
	

}

void ParticleObject::render()
{
	SDL_Rect *textureSourceRect = NULL, destRect;
	SDL_Texture* texture = NULL;

	//Get render destination rectangle
	destRect = this->getRenderDestRect();

	//Get texture
	texture = this->getRenderTexture(texture);

	//Get render texture src rectangle
	textureSourceRect = this->getRenderTextureRect(textureSourceRect);


	SDL_SetTextureAlphaMod(texture, this->color.a); //seems helpful for particles

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD); //alpha ADD seems good for particles

	//Set the render color based on the partticles' color
	SDL_SetTextureColorMod(texture, this->color.r, this->color.g, this->color.b);

	//Get the angle of the object and convert it from Radians to Degrees for SDL
	float angle = this->physicsBody->GetAngle();
	angle = angle * 180 / M_PI;

	WorldObject::render();
}

