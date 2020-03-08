#include "LevelManager.h"
#include "Globals.h"
#include "Texture.h"
#include "TextureManager.h"
#include "WorldObject.h"
#include "GameObjectManager.h"
#include "GameConfig.h"
#include "Game.h"



static const SDL_Color WHITE = { 255,255,255 };
static const SDL_Color BLACK = { 0,0,0 };

static const unsigned char wallOnLeft = 0b0001;
static const unsigned char wallOnRight = 0b0010;
static const unsigned char wallOnTop = 0b0100;
static const unsigned char wallOnBottom = 0b1000;

static const unsigned char open = 0b1111;
static const unsigned char topEnd = 0b1000;
static const unsigned char bottomEnd = 0b0100;
static const unsigned char topLeftCorner = 0b1010;
static const unsigned char topRightCorner = 0b1001;
static const unsigned char bottomLeftCorner = 0b0110;
static const unsigned char bottomRightCorner = 0b0101;
static const unsigned char hallHorz = 0b0011;
static const unsigned char hallVert = 0b1100;
static const unsigned char rightEnd = 0b0001;
static const unsigned char leftEnd = 0b0010;
static const unsigned char topWall = 0b1011;
static const unsigned char rightWall = 0b1101;
static const unsigned char bottomWall = 0b0111;
static const unsigned char leftWall = 0b1110;
static const unsigned char column = 0b0000;

void LevelManager::loadLevelBlueprint(string levelId)
{

	SDL_Texture* levelImage;
	SDL_PixelFormat *fmt;
	SDL_Color *color;
	SDL_Surface* surface;

	//Set the games current level
	game->currentLevel = levelId;

	//I am representing the level grid as a png image file 
	levelImage = TextureManager::instance().getTexture(levelId)->sdlTexture;
	surface = TextureManager::instance().getTexture(levelId)->surface;

	//Create the level object
	Level* level = new Level();
	level->id = levelId;

	level->width = surface->w;
	level->height = surface->h;

	SDL_LockSurface(surface);

	//Add level to the vector
	this->levels[levelId] = level;

	//Initialze the array based on the pixels in the image by resizing it to proper size
	this->levels[levelId]->levelObjects.resize(surface->w, vector<LevelObject>(surface->h));

	//Loop through entire image, top to bottom, left to right and build the
	//2 dimensional array of tile objects
	for(int y = 0; y< surface->h;y++)
		{
		for (int x = 0; x < surface->w; x++)
		{
			//determine what tile to build for current x,y location
			//and createTile game object
			LevelObject levelObject = *determineTile(x, y, surface);

			//Add levelItem to array
			this->levels[levelId]->levelObjects[x][y] = levelObject;

			//use the first levelItem to determine the tile size of the level and world
			if (x == 0 && y == 0)
			{
				
				level->tileWidth = GameObjectManager::instance().gameObjectDefinitions[levelObject.gameObjectId]->xSize
					* GameConfig::instance().scaleFactor();
				level->tileHeight = GameObjectManager::instance().gameObjectDefinitions[levelObject.gameObjectId]->ySize
					* GameConfig::instance().scaleFactor();
			}
		}
	}

	SDL_UnlockSurface(surface);
}

LevelObject* LevelManager::determineTile(int x, int y, SDL_Surface* surface)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 red, green, blue, alpha;
	LevelObject* levelObject = NULL;
	Uint8 *pixel = NULL;
	SDL_Color leftColor, rightColor, topColor, bottomColor;
	unsigned int borderWalls = 0;

	leftColor = rightColor = topColor = bottomColor = WHITE;

	//get the pixel at this location
	Uint8 *currentPixel = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	//Parse the pixel info into a color
	SDL_GetRGBA(*(Uint32*)currentPixel, surface->format, &red, &green, &blue, &alpha);
	SDL_Color currentPixelcolor = { red, green, blue };

	//Init level object
	levelObject = new LevelObject();
	levelObject->gameObjectId = "";

	//If this is a wall, get the pixel to the left, right, top, and bottom 
	//- check if we are on the edge while grabbing pixel 
	//if (memcmp(&currentPixelcolor, &BLACK, sizeof(SDL_Color)) == 0)
	if (currentPixelcolor == BLACK)
	{

		if (x != 0) {
			pixel = (Uint8 *)surface->pixels + y * surface->pitch + (x - 1) * bpp;
			SDL_GetRGBA(*(Uint32*)pixel, surface->format, &red, &green, &blue, &alpha);
			leftColor = { red, green, blue };
		}
		if (x != surface->w - 1) {
			pixel = (Uint8 *)surface->pixels + y * surface->pitch + (x + 1) * bpp;
			SDL_GetRGBA(*(Uint32*)pixel, surface->format, &red, &green, &blue, &alpha);
			rightColor = { red, green, blue };
		}
		if (y != 0) {
			pixel = (Uint8 *)surface->pixels + (y - 1) * surface->pitch + x * bpp;
			SDL_GetRGBA(*(Uint32*)pixel, surface->format, &red, &green, &blue, &alpha);
			topColor = { red, green, blue };
		}
		if (y != surface->h - 1) {
			pixel = (Uint8 *)surface->pixels + (y + 1) * surface->pitch + x * bpp;
			SDL_GetRGBA(*(Uint32*)pixel, surface->format, &red, &green, &blue, &alpha);
			bottomColor = { red, green, blue };
		}

		//Set the bit mask to match which walls exist where
		if (leftColor == BLACK) {
			borderWalls |= wallOnLeft;
		}
		if (rightColor == BLACK) {
			borderWalls |= wallOnRight;
		}
		if (topColor == BLACK) {
			borderWalls |= wallOnTop;
		}
		if (bottomColor == BLACK) {
			borderWalls |= wallOnBottom;
		}

		//build levelObject Here
		if (borderWalls == open)
		{
			levelObject->gameObjectId = "WALL1_OPEN";
		}
		else if (borderWalls == topEnd)
		{
			levelObject->gameObjectId = "WALL1_END";
		}
		else if (borderWalls == bottomEnd)
		{
			levelObject->gameObjectId = "WALL1_END";
			levelObject->angleAdjustment = 180;
		}
		else if (borderWalls == rightEnd)
		{
			levelObject->gameObjectId = "WALL1_END";
			levelObject->angleAdjustment = 90;
		}
		else if (borderWalls == leftEnd)
		{
			levelObject->gameObjectId = "WALL1_END";
			levelObject->angleAdjustment = -90;
		}
		else if (borderWalls == topLeftCorner)
		{
			levelObject->gameObjectId = "WALL1_CORNER";
		}
		else if (borderWalls == topRightCorner)
		{
			levelObject->gameObjectId = "WALL1_CORNER";
			levelObject->angleAdjustment = 90;
		}
		else if (borderWalls == bottomLeftCorner)
		{
			levelObject->gameObjectId = "WALL1_CORNER";
			levelObject->angleAdjustment = -90;
		}
		else if (borderWalls == bottomRightCorner)
		{
			levelObject->gameObjectId = "WALL1_CORNER";
			levelObject->angleAdjustment = 180;
		}
		else if (borderWalls == hallHorz)
		{
			levelObject->gameObjectId = "WALL1_HALL";
		}
		else if (borderWalls == hallVert)
		{
			levelObject->gameObjectId = "WALL1_HALL";
			levelObject->angleAdjustment = 90;
		}
		else if (borderWalls == topWall)
		{
			levelObject->gameObjectId = "WALL1_WALL";
		}
		else if (borderWalls == rightWall)
		{
			levelObject->gameObjectId = "WALL1_WALL";
			levelObject->angleAdjustment = 90;
		}
		else if (borderWalls == bottomWall)
		{
			levelObject->gameObjectId = "WALL1_WALL";
			levelObject->angleAdjustment = 180;
		}
		else if (borderWalls == leftWall)
		{
			levelObject->gameObjectId = "WALL1_WALL";
			levelObject->angleAdjustment = -90;
		}
		else if (borderWalls == column)
		{
			levelObject->gameObjectId = "WALL1_COLUMN";
		}
	}

	return levelObject;

}

void LevelManager::buildLevel(string levelId)
{
	Level* level = this->levels[levelId];
	LevelObject* levelObject;
	//unique_ptr<WorldObject> worldObject;
	WorldObject* worldObject;

	for (int y = 0; y < level->height; y++)
	{
		for (int x = 0; x < level->width; x++)
		{

			if (level->levelObjects[x][y].gameObjectId.empty() == false)
			{
				levelObject = &level->levelObjects[x][y];

				worldObject = GameObjectManager::instance().buildGameObject <WorldObject>(levelObject->gameObjectId, x, y, levelObject->angleAdjustment);
				game->addGameObject(worldObject, GameOjectLayer::MAIN);

			}

		}
	}
}


