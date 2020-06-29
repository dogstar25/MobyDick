#include "ParticleObject.h"




ParticleObject::ParticleObject()
{
}

ParticleObject::ParticleObject(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust) :
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
	std::chrono::steady_clock::time_point now_time = std::chrono::steady_clock::now();

	std::chrono::duration<double, std::milli> timeDiffMilliSeconds = now_time - this->time_snapshot;

	if (this->hasInfiniteLifetime == false)
	{
		this->lifetimeRemaining -= timeDiffMilliSeconds;

		if (this->lifetimeRemaining.count() <= 0)
		{

			//Mark this object for removal so that the removal loop will delete it
			this->setRemoveFromWorld(true);


		}
		else
		{
			this->time_snapshot = now_time;

			//If this particle should fade over time, then adjust its alpha value
			if (this->isLifetimeAlphaFade)
			{
				this->setColorAlpha( int(255 * (this->lifetimeRemaining / this->lifetime)));
			}


		}
	}
	

}

void ParticleObject::render()
{
	SDL_Texture* texture = NULL;

	//Set particles texture to a blend mode
	texture = this->getRenderTexture();
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);

	WorldObject::render();
}

