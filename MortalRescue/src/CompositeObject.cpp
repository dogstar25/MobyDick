#include "game.h"
#include "CompositeObject.h"


CompositeObject::CompositeObject(string gameObjectId, float xMapPos, float yMapPos, float angleAdjust) :
	WorldObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

	//this->blueprint = game->textureManager.getTexture(definition->blueprint);

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
	string blueprintTexureId;
	Uint8 red, green, blue, alpha;

	//Get the texture and the surface
	blueprintTexureId = this->definition->compositeDetails.blueprint.textureId;
	blueprintSurface = game->textureManager.getTexture(blueprintTexureId)->surface;
	int bpp = blueprintSurface->format->BytesPerPixel;

	SDL_LockSurface(blueprintSurface);

	//Loop through entire image, top to bottom, left to right and build the pieces
	for (int y = 0; y < blueprintSurface->h; y++)
	{
		for (int x = 0; x < blueprintSurface->w; x++)
		{

			//get the pixel at this location
			Uint8* currentPixel = (Uint8*)blueprintSurface->pixels + y * blueprintSurface->pitch + x * bpp;

			//Parse the pixel info into a color
			SDL_GetRGBA(*(Uint32*)currentPixel, blueprintSurface->format, &red, &green, &blue, &alpha);
			SDL_Color currentPixelcolor = { red, green, blue };

			//Loop through the legend to find which gameObject should be built
			for (CompositeLegendItem legendItem : this->definition->compositeDetails.blueprint.legend)
			{
				if (currentPixelcolor == legendItem.color == true)
				{
					this->buildPiece(legendItem, x, y);
				}
			}

		}
	}

	SDL_UnlockSurface(blueprintSurface);
}

void CompositeObject::buildPiece(CompositeLegendItem legendItem, int xPos, int yPos)
{
	float xOffset, yOffset;
	GameObjectPiece piece = {};
	piece.currentlevel = 1;
	piece.isDestroyed = false;
	piece.time_snapshot = steady_clock::now();

	/*
	Build the game objects off screen. They will be placed in exect location duriing update loop
	*/
	if (legendItem.gameObjectType.compare("WORLD_OBJECT") == 0)
		{
		WorldObject* worldObject =
			game->gameObjectManager.buildGameObject<WorldObject>(legendItem.gameObjectId, -5, -5, 0);
		piece.gameObject = make_shared<WorldObject>(*worldObject);
		
		}
	else //default to GAME_OBJECT
		{
		GameObject* gameObject =
			game->gameObjectManager.buildGameObject<GameObject>(legendItem.gameObjectId, -5, -5, 0);
		piece.gameObject = make_shared<GameObject>(*gameObject);
		}

	//calculate the X,Y offset position in relating to the base object
	SDL_Rect parentPositionRect = this->getPositionRect();
	xOffset = parentPositionRect.x + (xPos * piece.gameObject->xSize);
	yOffset = parentPositionRect.y + (yPos * piece.gameObject->ySize);

	piece.parentPositionOffset.x = xOffset;
	piece.parentPositionOffset.y = yOffset;

}

void CompositeObject::updatePieces()
{



}
