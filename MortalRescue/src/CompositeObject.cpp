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

	for (auto pieceObject : this->pieces)
	{
		if(pieceObject.isDestroyed == false)
		{
			pieceObject.gameObject->render();
		}
		
	}
	

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
		piece.gameObject = worldObject;
		
		}
	else //default to GAME_OBJECT
		{
		GameObject* gameObject =
			game->gameObjectManager.buildGameObject<GameObject>(legendItem.gameObjectId, -5, -5, 0);
		piece.gameObject = gameObject;
		}

	//calculate the X,Y offset position in relating to the base object
	//SDL_Rect parentPositionRect = this->getPositionRect();
	//xOffset = parentPositionRect.x + (xPos * piece.gameObject->xSize);
	//yOffset = parentPositionRect.y + (yPos * piece.gameObject->ySize);

	xOffset = xPos * piece.gameObject->xSize;
	yOffset = yPos * piece.gameObject->ySize;

	piece.parentPositionOffset.x = xOffset;
	piece.parentPositionOffset.y = yOffset;

	//Temp color setting
	piece.gameObject->color = { 255,0,0,255 };

	//Temp adjust render size
	//piece.gameObject->xSize += 2;
	//piece.gameObject->ySize += 2;

	this->pieces.push_back(piece);

}

void CompositeObject::updatePieces()
{
	for (auto& pieceObject : this->pieces)
	{

		//Update the state of the piece
		this->updatePieceState(pieceObject);

		//Update the position of the piece
		this->updatePiecePosition(pieceObject);

		//The piece object itself is a gameObject that should have its update called
		pieceObject.gameObject->update();

	}



}

void CompositeObject::updatePieceState(GameObjectPiece& piece)
{
	//Get now time
	steady_clock::time_point now_time = steady_clock::now();

	//Should this object be removed?
	if (piece.gameObject->removeFromWorld == true)
	{
		piece.gameObject->setActive(false);
		piece.isDestroyed = true;
		//piece.gameObject->color = { 0,255,0,255 };
		piece.time_snapshot = now_time;
	}

	//Has enough time gone by to regenerate the next armor level
	if (piece.isDestroyed == true)
	{
		duration<double, milli> timeDiffMilliSeconds = now_time - piece.time_snapshot;
		if (timeDiffMilliSeconds.count() >= this->definition->compositeDetails.levelUpSpeed)
		{

			//write regen function

		}


	}


}

void CompositeObject::updatePiecePosition(GameObjectPiece& piece)
{

	b2Vec2 piecePosition{ 0,0 };
	b2Vec2 adjustment{ 0,0 };

	//calculate the X,Y offset position in relating to the base object
	SDL_Rect parentPositionRect = this->getPositionRect();

	//piecePositions
	SDL_Rect piecePositionRect{};
	piecePositionRect.x = parentPositionRect.x + piece.parentPositionOffset.x;
	piecePositionRect.y = parentPositionRect.y + piece.parentPositionOffset.y;
	piecePositionRect.w = piece.gameObject->xSize;
	piecePositionRect.h = piece.gameObject->ySize;

	//Adjust the piece position based on the base objects rotation/angle
	adjustment = this->matchParentRotation(piecePositionRect, parentPositionRect, this->angle);
	piecePositionRect.x += adjustment.x;
	piecePositionRect.y += adjustment.y;

	//Create a vec position object to pass to setPosition
	piecePosition.x = piecePositionRect.x;
	piecePosition.y = piecePositionRect.y;
	piece.gameObject->setPosition(piecePosition, this->angle);
}
