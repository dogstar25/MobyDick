#include "LevelManager.h"

#include "triggers/Trigger.h"
#include "triggers/TriggerMap.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "EnumMaps.h"
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


LevelManager::LevelManager()
{

}

LevelManager::~LevelManager()
{

}

LevelManager& LevelManager::instance()
{
	static LevelManager singletonInstance;
	return singletonInstance;
}

void LevelManager::addLevelObject(int xIndex, int yIndex, LevelObject levelObject)
{

	m_levelObjects[xIndex][yIndex]=levelObject;

}

void LevelManager::setLevelObjectArraySize(int width, int height)
{

	m_levelObjects.resize(width, std::vector<LevelObject>(height));

}

void LevelManager::_loadDefinition(std::string levelId)
{
	//Read file and stream it to a JSON object
	std::string filename = "assets/levels/" + levelId + "_definition.json";

	Json::Value root;
	std::ifstream ifs(filename);
	ifs >> root;

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

	//Save all locationobject definitions
	if (root.isMember("locationObjects")) {
		m_locationList = root["locationObjects"];
	}

	//Get all trigger items
	if (root.isMember("levelTriggers")) {

		for (Json::Value itrTrigger : root["levelTriggers"])
		{
			//Get the name of the class to be used as the action as a string
			std::string triggerId = itrTrigger["triggerClass"].asString();
			std::shared_ptr<Trigger> tempTrigger = TriggerMap::instance().getTrigger(triggerId);
			m_levelTriggers.emplace_back(std::move(tempTrigger));

		}

	}

}

void LevelManager::update(Scene* scene)
{

	for (const auto& trigger : m_levelTriggers) {

		if (trigger->hasMetCriteria()) {
			trigger->execute();
		}
	}

}

void LevelManager::clearTriggers()
{

	//Clear triggers
	m_levelTriggers.clear();



}

void LevelManager::load(std::string levelId, Scene* scene)
{

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

	Game::instance()._displayLoadingMsg();

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
			auto levelObject = _determineTile(x, y, surface);

			//If a valid gameObject was found at this location then store its Id
			if (levelObject.has_value()) {
				m_levelObjects[x][y] = levelObject.value();
			}
		}
	}

	SDL_UnlockSurface(surface);

	Game::instance()._displayLoadingMsg();

	//Build all of the objects that make up this level and store them
	//In the main gameObject collection
	_buildLevelObjects(scene);

	//Clear the level objects collection now that all gameObjects are built
	m_levelObjects.clear();

}

std::optional<LevelObject> LevelManager::_determineTile(int x, int y, SDL_Surface* surface)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 red, green, blue, alpha;
	std::optional<LevelObject> levelObject{ std::nullopt };
	Uint8* pixel = NULL;
	SDL_Color leftColor, rightColor, topColor, bottomColor;
	unsigned int borderWalls = 0;

	leftColor = rightColor = topColor = bottomColor = WHITE;

	//get the pixel at this location
	Uint8* currentPixel = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

	//Parse the pixel info into a color
	SDL_GetRGBA(*(Uint32*)currentPixel, surface->format, &red, &green, &blue, &alpha);
	SDL_Color currentPixelcolor = { red, green, blue };

	//If this is a wall, determine the wall object, otherwise lookup the object in the level definition file
	if (currentPixelcolor == BLACK)	{
		levelObject = _determineWallObject(x,y,surface);
	}
	else if (currentPixelcolor != WHITE) {
		levelObject = _determineLocationObject(x, y, surface);
	}

	return levelObject;

}

std::optional<LevelObject> LevelManager::_determineLocationObject(int x, int y, SDL_Surface* bluePrintSurface)
{

	std::stringstream levellocationObjectId;
	levellocationObjectId << "LOC" << "_" << std::setw(4) << std::setfill('0') << x << "_";
	levellocationObjectId << std::setw(4) << std::setfill('0') << y;
	std::string levelObjectId = levellocationObjectId.str();
	std::optional<LevelObject> levelObject{ };

	//This location should have a location item defined for it 
	for (Json::Value locationItemJSON : m_locationList) {

		std::string id = locationItemJSON["id"].asString();
		if (levelObjectId == id) {

			levelObject = LevelObject();
			levelObject->gameObjectId = locationItemJSON["gameObjectId"].asString();
			if (locationItemJSON.isMember("layer")) {
				levelObject->layer = EnumMap::instance().toEnum(locationItemJSON["layer"].asString());
			}
			levelObject->cameraFollow = locationItemJSON["cameraFollow"].asBool();
			break;
		}
	}

	if (levelObject.has_value() == false) {
		std::cout << "WARNING: Blueprint LevelObject found at " << x << " " << y << " " << " found with no definition.\n";
	}
	
	return levelObject;
}


LevelObject LevelManager::_determineWallObject(int x, int y, SDL_Surface* bluePrintSurface)
{

	int bpp{ bluePrintSurface->format->BytesPerPixel };
	Uint8 red, green, blue, alpha;
	LevelObject levelObject{};
	Uint8* pixel = { NULL };
	SDL_Color leftColor = { 255,255,255 }, rightColor = { 255,255,255 }, topColor = { 255,255,255 }, bottomColor = { 255,255,255 };
	unsigned int borderWalls{ 0 };

	if (x != 0) {
		pixel = (Uint8*)bluePrintSurface->pixels + y * bluePrintSurface->pitch + (x - 1) * bpp;
		SDL_GetRGBA(*(Uint32*)pixel, bluePrintSurface->format, &red, &green, &blue, &alpha);
		leftColor = { red, green, blue };
	}
	if (x != bluePrintSurface->w - 1) {
		pixel = (Uint8*)bluePrintSurface->pixels + y * bluePrintSurface->pitch + (x + 1) * bpp;
		SDL_GetRGBA(*(Uint32*)pixel, bluePrintSurface->format, &red, &green, &blue, &alpha);
		rightColor = { red, green, blue };
	}
	if (y != 0) {
		pixel = (Uint8*)bluePrintSurface->pixels + (y - 1) * bluePrintSurface->pitch + x * bpp;
		SDL_GetRGBA(*(Uint32*)pixel, bluePrintSurface->format, &red, &green, &blue, &alpha);
		topColor = { red, green, blue };
	}
	if (y != bluePrintSurface->h - 1) {
		pixel = (Uint8*)bluePrintSurface->pixels + (y + 1) * bluePrintSurface->pitch + x * bpp;
		SDL_GetRGBA(*(Uint32*)pixel, bluePrintSurface->format, &red, &green, &blue, &alpha);
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
		levelObject.gameObjectId = "WALL1_OPEN";
	}
	else if (borderWalls == topEnd)
	{
		levelObject.gameObjectId = "WALL1_END";
	}
	else if (borderWalls == bottomEnd)
	{
		levelObject.gameObjectId = "WALL1_END";
		levelObject.angleAdjustment = 180;
	}
	else if (borderWalls == rightEnd)
	{
		levelObject.gameObjectId = "WALL1_END";
		levelObject.angleAdjustment = 90;
	}
	else if (borderWalls == leftEnd)
	{
		levelObject.gameObjectId = "WALL1_END";
		levelObject.angleAdjustment = -90;
	}
	else if (borderWalls == topLeftCorner)
	{
		levelObject.gameObjectId = "WALL1_CORNER";
	}
	else if (borderWalls == topRightCorner)
	{
		levelObject.gameObjectId = "WALL1_CORNER";
		levelObject.angleAdjustment = 90;
	}
	else if (borderWalls == bottomLeftCorner)
	{
		levelObject.gameObjectId = "WALL1_CORNER";
		levelObject.angleAdjustment = -90;
	}
	else if (borderWalls == bottomRightCorner)
	{
		levelObject.gameObjectId = "WALL1_CORNER";
		levelObject.angleAdjustment = 180;
	}
	else if (borderWalls == hallHorz)
	{
		levelObject.gameObjectId = "WALL1_HALL";
	}
	else if (borderWalls == hallVert)
	{
		levelObject.gameObjectId = "WALL1_HALL";
		levelObject.angleAdjustment = 90;
	}
	else if (borderWalls == topWall)
	{
		levelObject.gameObjectId = "WALL1_WALL";
	}
	else if (borderWalls == rightWall)
	{
		levelObject.gameObjectId = "WALL1_WALL";
		levelObject.angleAdjustment = 90;
	}
	else if (borderWalls == bottomWall)
	{
		levelObject.gameObjectId = "WALL1_WALL";
		levelObject.angleAdjustment = 180;
	}
	else if (borderWalls == leftWall)
	{
		levelObject.gameObjectId = "WALL1_WALL";
		levelObject.angleAdjustment = -90;
	}
	else if (borderWalls == column)
	{
		levelObject.gameObjectId = "WALL1_COLUMN";
	}
	return levelObject;

}


void LevelManager::_buildLevelObjects(Scene* scene)
{
	LevelObject* levelObject;

	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {

			if (m_levelObjects[x][y].gameObjectId.empty() == false) {

				levelObject = &m_levelObjects[x][y];
				scene->addGameObject(levelObject->gameObjectId, levelObject->layer,
					(float)x, (float)y, (float)levelObject->angleAdjustment, levelObject->cameraFollow);

			}

		}
	}
}

