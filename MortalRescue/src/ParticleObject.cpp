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

	//this->time_snapshot = steady_clock::now();



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
		game->debugPanel->addItem("RESET_PARTICLE", "TRUE");

		//Mark this object for removal so that the removal loop will delete it
		this->removeFromWorld = true;

	}
	else
	{
		this->time_snapshot = now_time;
		game->debugPanel->addItem("RESET_PARTICLE", "FALSE");
	}
	

}

void ParticleObject::render()
{
	SDL_Rect *textureSourceRect = NULL, destRect;
	SDL_Texture* texture = NULL;

	//Get render destination rectangle
	this->getRenderDestRect(&destRect);

	//Get texture
	texture = this->getRenderTexture(texture);

	//Get render texture src rectangle
	textureSourceRect = this->getRenderTextureRect(textureSourceRect);


	//SDL_SetTextureAlphaMod(texture, randomInt); //seems helpful for particles

	//SDL_GetTextureColorMod(texture, &r, &g, &b);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD); //alpha ADD seems good for particles
	//SDL_RenderSetScale(pRenderer, randomInt, randomInt); // nope


//	if (CHECK A NEEDED ISCOLOR VARIABLE FLAG) {
	/*
		int randomR = game->util.generateRandomNumber(204, 224);
		int randomG = game->util.generateRandomNumber(8, 89);
		int randomB = game->util.generateRandomNumber(8, 11);
		SDL_SetTextureColorMod(texture, randomR, randomG, randomB);
*/
	//}

	//Get the angle of the object and convert it from Radians to Degrees for SDL
	float angle = this->physicsBody->GetAngle();
	angle = angle * 180 / M_PI;

	WorldObject::render();
}

