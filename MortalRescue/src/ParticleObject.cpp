#include "ParticleObject.h"
#include "game.h"
#include <string>
#include <json/json.h>

using namespace std::chrono_literals;
using namespace std;

ParticleObject::ParticleObject()
{
}

ParticleObject::ParticleObject(string gameObjectId, int xMapPos, int yMapPos, int angleAdjust) :
	WorldObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

	this->lifetime = (definition->lifetime);



}

ParticleObject::~ParticleObject()
{
}


void ParticleObject::update()
{
	GameObject::update();
	//seconds lifeTimeSeconds = this->lifetime;
	/*
	steady_clock::time_point now_time = steady_clock::now();
	duration<double, milli> timeDiffMilliSeconds = now_time - this->time_snapshot;
	std::chrono::duration<float> timeDiffSeconds = std::chrono::duration_cast<std::chrono::seconds>(timeDiffMilliSeconds);

	//debug
	game->debugPanel->addItem("LIFETIME", to_string(this->lifetime));
	game->debugPanel->addItem("PART_TIMEDIFF_MILI", to_string(timeDiffMilliSeconds.count()));
	game->debugPanel->addItem("PART_TIMEDIFF_SEC", to_string(timeDiffSeconds.count()));

	this->lifetimeRemaining -= timeDiffSeconds.count();

	if (this->lifetimeRemaining <= 0)
	{

		//reset the particle object
		game->objectPoolManager.reset(this);
		game->debugPanel->addItem("RESET_PARTICLE", "TRUE");

		//Mark this object for removal so that the removal loop will delete it
		this->removeFromWorld = true;

	}
	else
	{
		this->time_snapshot = now_time;
		game->debugPanel->addItem("RESET_PARTICLE", "FALSE");
	}
	*/
	

}

void ParticleObject::render()
{

	game->textureManager.render(this);
}

