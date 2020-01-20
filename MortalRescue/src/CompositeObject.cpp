#include "game.h"
#include "CompositeObject.h"


CompositeObject::CompositeObject(string gameObjectId, float xMapPos, float yMapPos, float angleAdjust) :
	WorldObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

	this->blueprint = game->textureManager.getTexture(definition->blueprint);

	this->buildComposite();

}

CompositeObject::~CompositeObject()
{


}
void CompositeObject::update()
{
	//Call base game object update
	WorldObject::update();

	this->updatePieces();
}

void CompositeObject::render()
{
	WorldObject::render();

	/* from particle render
	SDL_SetTextureAlphaMod(texture, this->color.a); //seems helpful for particles

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD); //alpha ADD seems good for particles

	//Set the render color based on the partticles' color
	SDL_SetTextureColorMod(texture, this->color.r, this->color.g, this->color.b);
	*/

}

void CompositeObject::buildComposite()
{

	SDL_Texture* blueprintTexure;
	SDL_Surface* blueprintSurface;
	SDL_PixelFormat* fmt;
	SDL_Color* color;

	//Get the texture and the surface
	blueprintTexure = blueprint->sdlTexture;
	blueprintSurface = blueprint->surface;

	SDL_LockSurface(blueprintSurface);

	//Loop through entire image, top to bottom, left to right and build the pieces
	for (int y = 0; y < blueprintSurface->h; y++)
	{
		for (int x = 0; x < blueprintSurface->w; x++)
		{

			if (true)
			{




			}




		}
	}

	SDL_UnlockSurface(blueprintSurface);
}


void CompositeObject::updatePieces()
{



}
