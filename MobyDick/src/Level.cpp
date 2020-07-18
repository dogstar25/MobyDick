#include "Level.h"

#include <iostream>
#include <fstream>

#include <json/json.h>

#include "Globals.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
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


Level& Level::instance()
{
	static Level singletonInstance;
	return singletonInstance;
}

void Level::addWaypoint(Waypoint wayPoint)
{

	m_waypoints.push_back(wayPoint);


}

void Level::addLevelObject(int xIndex, int yIndex, LevelObject levelObject)
{

	levelObjects[xIndex][yIndex]=levelObject;

}

void Level::setLevelObjectArraySize(int width, int height)
{

	levelObjects.resize(width, std::vector<LevelObject>(height));

}

void Level::_loadDefinition(std::string levelId)
{
	//Read file and stream it to a JSON object
	bool success = false;
	Json::CharReaderBuilder jsonBuilder;
	Json::Value root;
	std::string filename = "assets/levels/" + levelId + "_definition.json";
	std::string errors;
	std::ifstream ifs(filename);

	success = Json::parseFromStream(jsonBuilder, ifs, &root, &errors);

	if (success == true)
	{
		//Level definition values
		m_id = levelId;
		m_description = root["description"].asString();
		m_blueprint = root["blueprint"].asString();

		//Dimensions
		m_width = root["dimensions"]["levelWidth"].asInt();
		m_height = root["dimensions"]["levelHeight"].asInt();
		m_tileWidth = root["dimensions"]["tileWidth"].asInt();
		m_tileHeight = root["dimensions"]["tileHeight"].asInt();
		m_levelBounds.x = 0;
		m_levelBounds.y = 0;
		m_levelBounds.w = m_width * m_tileWidth;
		m_levelBounds.h = m_height * m_tileHeight;

		//Initialize World bounds
		Game::instance().setWorldParams(m_levelBounds, m_tileWidth, m_tileHeight);

		LevelObject* locationDefinition = NULL;
		std::string locationId;
		for (auto itr : root["locationObjects"])
		{
			locationDefinition = new LevelObject();
			locationDefinition->gameObjectId = itr["gameObjectId"].asString();
			locationId = itr["id"].asString();
			m_locationObjects.emplace(locationId, locationDefinition);

		}
			
	}
	else
	{
		//TODO: error logger
	}
}

void Level::load(std::string levelId)
{

	SDL_Texture* levelImage;
	SDL_PixelFormat* fmt;
	SDL_Color* color;
	SDL_Surface* surface;

	//Load the Level definition
	_loadDefinition(levelId);

	//I am representing the level grid as a png image file 
	surface = TextureManager::instance().getTexture(m_blueprint)->surface;

	//Log warning if the bluprint image size doesnt match what we ahve in config
	int surfaceWidth = surface->w;
	int surfaceHeight = surface->h;
	if (surfaceWidth != m_width ||
		surfaceHeight != m_height)
	{
		std::cout << "WARNING: Blueprint " << m_id << " width/height: " << surfaceWidth << "/" << surfaceHeight << " does not match defined width/height of: " 
			<<	m_width << "/" << m_height << "\n";
	}

	SDL_LockSurface(surface);

	//Allocate the 2 dimentional vector
	setLevelObjectArraySize(m_width, m_height);

	//Loop through entire image, top to bottom, left to right and build the
	//2 dimensional array of tile objects
	for (int y = 0; y < surface->h; y++)
	{
		for (int x = 0; x < surface->w; x++)
		{
			//determine what tile to build for current x,y location
			LevelObject levelObject = *_determineTile(x, y, surface);

			//Add levelItem to array
			levelObjects[x][y] = levelObject;

		}
	}

	SDL_UnlockSurface(surface);

	//Build all of the objects that make up this level and store them
	//In the main gameObject collection
	_buildLevelObjects();
}

LevelObject* Level::_determineTile(int x, int y, SDL_Surface* surface)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 red, green, blue, alpha;
	LevelObject* levelObject = NULL;
	Uint8* pixel = NULL;
	SDL_Color leftColor, rightColor, topColor, bottomColor;
	unsigned int borderWalls = 0;

	leftColor = rightColor = topColor = bottomColor = WHITE;

	//get the pixel at this location
	Uint8* currentPixel = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

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
			pixel = (Uint8*)surface->pixels + y * surface->pitch + (x - 1) * bpp;
			SDL_GetRGBA(*(Uint32*)pixel, surface->format, &red, &green, &blue, &alpha);
			leftColor = { red, green, blue };
		}
		if (x != surface->w - 1) {
			pixel = (Uint8*)surface->pixels + y * surface->pitch + (x + 1) * bpp;
			SDL_GetRGBA(*(Uint32*)pixel, surface->format, &red, &green, &blue, &alpha);
			rightColor = { red, green, blue };
		}
		if (y != 0) {
			pixel = (Uint8*)surface->pixels + (y - 1) * surface->pitch + x * bpp;
			SDL_GetRGBA(*(Uint32*)pixel, surface->format, &red, &green, &blue, &alpha);
			topColor = { red, green, blue };
		}
		if (y != surface->h - 1) {
			pixel = (Uint8*)surface->pixels + (y + 1) * surface->pitch + x * bpp;
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

void Level::_buildLevelObjects()
{
	LevelObject* levelObject;

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{

			if (levelObjects[x][y].gameObjectId.empty() == false)
			{
				levelObject = &levelObjects[x][y];

				Game::instance().addGameObject(levelObject->gameObjectId, GameObjectLayer::MAIN, x, y, levelObject->angleAdjustment);

			}

		}
	}
}
