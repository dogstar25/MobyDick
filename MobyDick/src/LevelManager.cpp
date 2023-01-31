#include "LevelManager.h"

#include <fstream>

#include "EnumMap.h"
#include "Game.h"
#include "ColorMap.h"
#include <assert.h>
#include "BaseConstants.h"
#include "SoundManager.h"



extern std::unique_ptr<Game> game;

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

	Json::Value levelsJSON;
	std::ifstream ifs("assets/levels/levels.json" );
	ifs >> levelsJSON;

	for (auto level : levelsJSON["levels"]) {
		std::string levelName = level.asString();
		m_levels.push_back(levelName);
	}


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
	std::stringstream filename;

	filename << "assets/levels/" << levelId << "_definition.json";

	Json::Value root;
	std::ifstream ifs(filename.str());
	if (ifs.fail() == true) {
		return;
	}

	ifs >> root;
	m_levelDefinition = root;

	//Level definition values
	//m_id = levelId;
	m_description = root["description"].asString();
	m_blueprintTexture = root["blueprint"].asString();

	if (root.isMember("wallColor")) {
		m_wallColor = game->colorMap()->toSDLColor(root["wallColor"].asString());
	}

	//Background Music
	if (root.isMember("backgroundMusic")) {
		m_backgroundMusicAssetId = root["backgroundMusic"].asString();
	}

	//Dimensions
	m_width = root["dimensions"]["levelWidth"].asInt();
	m_height = root["dimensions"]["levelHeight"].asInt();
	m_tileWidth = root["dimensions"]["tileWidth"].asInt();
	m_tileHeight = root["dimensions"]["tileHeight"].asInt();
	m_levelBounds.x = 0;
	m_levelBounds.y = 0;
	m_levelBounds.w = m_width * m_tileWidth;
	m_levelBounds.h = (m_height * m_tileHeight) + m_tileHeight + 8;

	//Background tiles?
	if (root.isMember("tiledLayers")){

		for (auto& tiledLayer : root["tiledLayers"]) {

			TiledLayerDefinition tiledLayerDefinition;

			tiledLayerDefinition.layerId = game->enumMap()->toEnum(tiledLayer["layer"].asString());
			tiledLayerDefinition.tiledObjectId = tiledLayer["tiledObjectId"].asString();
			tiledLayerDefinition.tileSize = { tiledLayer["tileSize"]["width"].asInt() , tiledLayer["tileSize"]["height"].asInt() };

			m_tiledLayerDefinitions.emplace(tiledLayerDefinition.layerId, tiledLayerDefinition);
		}

	}

	//Initialize World bounds
	game->setWorldParams(m_levelBounds, { m_tileWidth, m_tileHeight });

	//Save a
	//Save all color defined definitions
	if (root.isMember("colorDefinedObjects")) {
		m_colorDefinedList = root["colorDefinedObjects"];
	}

	//Save all locationobject definitions
	if (root.isMember("locationDefinedObjects")) {
		m_locationDefinedList = root["locationDefinedObjects"];
	}


}

void LevelManager::loadLevel(std::string levelId, std::string sceneId)
{

	//Find the scene indicated
	for (auto& scene : SceneManager::instance().scenes()) {
		if (scene.id() == sceneId) {
			scene.loadLevel(levelId);
		}
	}
	
	assert(false && "No Scene found for loading level");

}

std::optional<std::string> LevelManager::getNextLevelId(std::string currentLevelId)
{
	std::optional<std::string> nextLevelId{};


	//auto it = m_levels.begin();
	//while (it != m_levels.end()) {

	//	if (*it == currentLevelId && (it+1) != m_levels.end()) {
	//		nextLevelId = *(it + 1);
	//		break;
	//	}
	//	else{
	//		++it;
	//	}

	//	
	//}

	bool useNextItem = false;
	for (auto& level : m_levels) {

		if (useNextItem == true) {
			nextLevelId = level;
			break;
		}

		if (level == currentLevelId) {
			useNextItem = true;
		}

	}

	return nextLevelId;
}

void LevelManager::loadLevel(std::string levelId, Scene* scene)
{

	SDL_Surface* surface;

	//Load the Level definition
	_loadDefinition(levelId);

	//I am representing the level grid as a png image file 
	surface = TextureManager::instance().getTexture(m_blueprintTexture)->surface;

	//Log warning if the bluprint image size doesnt match what we ahve in config
	int surfaceWidth = surface->w;
	int surfaceHeight = surface->h;
	if (surfaceWidth != m_width ||
		surfaceHeight != m_height)
	{
		std::cout << "WARNING: Blueprint " << m_id << " width/height: " << surfaceWidth << "/" << surfaceHeight << " does not match defined width/height of: "
			<< m_width << "/" << m_height << "\n";
	}

	game->_displayLoadingMsg();

	SDL_LockSurface(surface);

	//Allocate the 2 dimentional vector for the level object 2d array
	setLevelObjectArraySize(m_width, m_height);

	//Allocate the 2 dimentional vector for the level nagigation map 2d array
	scene->setNavigationMapArraySize(m_width, m_height);

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

	game->_displayLoadingMsg();

	//Build all of the objects that make up this level and store them
	//In the main gameObject collection
	_buildLevelObjects(scene);

	//Build theDebug_grid_level objects
	_buildDebugGridObjects(scene);

	//Build the background image/objects
	_buildTiledLayers(scene);

	//Build the level Status Items
	_buildLevelStatusItems(levelId);

	//Build the level objectives
	_buildLevelObjectives(scene);

	//Build the level triggers
	_buildLevelTriggers(scene);

	//Build Level Parallax values
	_buildParallax(scene);

	//Build Level Cage
	_buildLevelCage(scene);

	//Build background music object
	_startBackgroundMusic();

	//Clear the level objects collection now that all gameObjects are built
	_clear();

}

void LevelManager::_clear()
{
	m_levelObjects.clear();
	m_tiledLayerDefinitions.clear();
	m_colorDefinedList.clear();
	m_locationDefinedList.clear();
}

void LevelManager::_buildDebugGridObjects(Scene* scene)
{

	//build all of the initial , blank, grid objects
	for (auto x = 0; x < m_width;x++) {
		for (auto y = 0; y < m_height; y++) {

			const auto& gridObject = scene->addGameObject("DEBUG_GRID_32", GameLayer::GRID_DISPLAY, (float)x, (float)y, (float)0);
			gridObject->disableRender();


		}
	}

}

void LevelManager::_startBackgroundMusic()
{

	//If there is background music specified then play it
	if (m_backgroundMusicAssetId.has_value()) {

		SoundManager::instance().playMusic(m_backgroundMusicAssetId.value(), -1);
	}



}

void LevelManager::_buildLevelCage(Scene* scene)
{

	Json::Value componentsDefinition{};
	Json::Value transformDefinition{};
	Json::Value physicsDefinition{};
	Json::Value fixtureDefinition{};


	auto levelWidth = (m_tileWidth * m_width) ;
	auto levelHeight = (m_tileHeight * m_height) ;

	//Create a Level Cage game object
	const auto& levelCageObject = scene->addGameObject("LEVEL_CAGE", GameLayer::ABSTRACT, (float)0, (float)0, (float)0);

	//Create a transform component dynamically
	transformDefinition["id"] = "TRANSFORM_COMPONENT";
	transformDefinition["size"]["width"] = levelWidth;
	transformDefinition["size"]["height"] = levelHeight;
	componentsDefinition["components"].append(transformDefinition);

	//Create a physics component dynamically
	physicsDefinition["id"] = "PHYSICS_COMPONENT";
	physicsDefinition["type"] = "B2_STATIC";

	{
		fixtureDefinition["contactTag"] = "ContactTag::WALL";
		fixtureDefinition["collisionShape"] = "B2_CHAIN";
		fixtureDefinition["physicsChainType"] = "PhysicsChainType::CCW_REFLECT_IN";
	}

	physicsDefinition["fixtures"].append(fixtureDefinition);
	componentsDefinition["components"].append(physicsDefinition);

	//Add the transform component
	const auto& transformComponent =
		std::static_pointer_cast<TransformComponent>(
			game->componentFactory()->create(componentsDefinition, "", "", scene, 0, 0, 0, ComponentTypes::TRANSFORM_COMPONENT)
			);
	transformComponent->setParent(levelCageObject);
	levelCageObject->addComponent(transformComponent);


	//Add the physics component
	const auto& physicsComponent =
		std::static_pointer_cast<PhysicsComponent>(
			game->componentFactory()->create(componentsDefinition, "", "", scene, 0, 0, 0, ComponentTypes::PHYSICS_COMPONENT)
			);
	physicsComponent->setParent(levelCageObject);
	levelCageObject->addComponent(physicsComponent);


}

void LevelManager::_buildParallax(Scene* scene)
{

	//Parallax
	if (m_levelDefinition.isMember("parallax")) {

		for (auto& parallax : m_levelDefinition["parallax"]) {

			Parallax parallaxItem;

			parallaxItem.layer = game->enumMap()->toEnum(parallax["layer"].asString());
			parallaxItem.rate = parallax["parallaxRate"].asFloat() * (float).01;

			scene->addParallaxItem(parallaxItem);
		}

	}



}

std::optional<LevelObject> LevelManager::_determineTile(int x, int y, SDL_Surface* surface)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 red, green, blue, alpha;
	std::optional<LevelObject> levelObject{};
	//levelObject = std::nullopt;
	Uint8* pixel = NULL;
	SDL_Color leftColor, rightColor, topColor, bottomColor;
	unsigned int borderWalls = 0;

	leftColor = rightColor = topColor = bottomColor = Colors::WHITE;

	//get the pixel at this location
	Uint8* currentPixel = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

	//Parse the pixel info into a color
	SDL_GetRGBA(*(Uint32*)currentPixel, surface->format, &red, &green, &blue, &alpha);
	SDL_Color currentPixelcolor = { red, green, blue };

	//Black pixels are walls
	if (currentPixelcolor == Colors::BLACK)	{
		levelObject = _determineWallObject(x,y,surface);
	}

	//If this is NOT white, then its either a color defined object or a locationdefined object
	//both are defined in the levelX_definition.json file
	else if (currentPixelcolor != Colors::WHITE) {
		if (_isColorDefinedObject(currentPixelcolor)) {
			levelObject = _determineColorDefinedObject(currentPixelcolor);
		}
		else {
			levelObject = _determineLocationDefinedObject(x, y);
		}
	}

	return levelObject;

}

std::optional<LevelObject> LevelManager::_determineColorDefinedObject(SDL_Color color)
{

	std::optional<LevelObject> levelObject{ };

	//This location should have a location item defined for it 
	for (Json::Value colorItemJSON : m_colorDefinedList) {

		SDL_Color colorKey = game->colorMap()->toSDLColor(colorItemJSON["colorKey"].asString());
		if (colorKey == color) {

			levelObject = LevelObject();
			levelObject->gameObjectType = colorItemJSON["gameObjectType"].asString();
			if (colorItemJSON.isMember("layer")) {
				levelObject->layer = game->enumMap()->toEnum(colorItemJSON["layer"].asString());
			}
			if (colorItemJSON.isMember("cameraFollow")) {
				levelObject->cameraFollow = colorItemJSON["cameraFollow"].asBool();
			}
			if (colorItemJSON.isMember("name")) {
				levelObject->name = colorItemJSON["name"].asString();
			}
			if (colorItemJSON.isMember("angle")) {
				levelObject->angleAdjustment = colorItemJSON["angle"].asInt();
			}
			if (colorItemJSON.isMember("objectColor")) {
				levelObject->color = game->colorMap()->toSDLColor(colorItemJSON["objectColor"].asString());
			}
			if (colorItemJSON.isMember("disabled")) {
				levelObject->disabledType = game->enumMap()->toEnum(colorItemJSON["disabled"].asString());
			}
			if (colorItemJSON.isMember("weaponForce")) {
				levelObject->weaponForce = colorItemJSON["weaponForce"].asFloat();
			}
			if (colorItemJSON.isMember("weaponColor")) {
				levelObject->weaponColor = game->colorMap()->toSDLColor(colorItemJSON["weaponColor"].asString());
			}
			if (colorItemJSON.isMember("compositePieceLevelCap")) {
				levelObject->compositePieceLevelCap = colorItemJSON["compositePieceLevelCap"].asInt();
			}
			if (colorItemJSON.isMember("brainSensorSize")) {
				levelObject->brainSensorSize = colorItemJSON["brainSensorSize"].asInt();
			}
			if (colorItemJSON.isMember("containerRespawnTimer")) {
				levelObject->containerRespawnTimer = colorItemJSON["containerRespawnTimer"].asFloat();
			}
			if (colorItemJSON.isMember("containerStartCount")) {
				levelObject->containerStartCount = colorItemJSON["containerStartCount"].asFloat();
			}
			if (colorItemJSON.isMember("containerCapacity")) {
				levelObject->containerCapacity = colorItemJSON["containerCapacity"].asFloat();
			}

		}
	}

	if (levelObject.has_value() == false) {
		std::cout << "WARNING: Color Defined Blueprint LevelObject found with no definition.\n";
	}
	
	return levelObject;
}

std::optional<LevelObject> LevelManager::_determineLocationDefinedObject(int x, int y)
{

	std::stringstream levellocationObjectId;
	levellocationObjectId << "LOC" << "_" << std::setw(4) << std::setfill('0') << x << "_";
	levellocationObjectId << std::setw(4) << std::setfill('0') << y;
	std::string levelObjectId = levellocationObjectId.str();
	std::optional<LevelObject> levelObject{ };

	//This location should have a location item defined for it 
	for (Json::Value locationItemJSON : m_locationDefinedList) {

		std::string id = locationItemJSON["id"].asString();
		if (levelObjectId == id) {

			levelObject = LevelObject();
			levelObject->gameObjectType = locationItemJSON["gameObjectType"].asString();

			if (locationItemJSON.isMember("layer")) {
				levelObject->layer = game->enumMap()->toEnum(locationItemJSON["layer"].asString());
			}
			if (locationItemJSON.isMember("cameraFollow")) {
				levelObject->cameraFollow = locationItemJSON["cameraFollow"].asBool();
			}
			if (locationItemJSON.isMember("name")) {
				levelObject->name = locationItemJSON["name"].asString();
			}
			if (locationItemJSON.isMember("angle")) {
				levelObject->angleAdjustment = locationItemJSON["angle"].asInt();
			}
			if (locationItemJSON.isMember("objectColor")) {
				levelObject->color = game->colorMap()->toSDLColor(locationItemJSON["objectColor"].asString());
			}
			if (locationItemJSON.isMember("disabled")) {
				levelObject->disabledType = game->enumMap()->toEnum(locationItemJSON["disabled"].asString());
			}
			if (locationItemJSON.isMember("weaponForce")) {
				levelObject->weaponForce = locationItemJSON["weaponForce"].asFloat();
			}
			if (locationItemJSON.isMember("weaponColor")) {
				levelObject->weaponColor = game->colorMap()->toSDLColor(locationItemJSON["weaponColor"].asString());
			}
			if (locationItemJSON.isMember("compositePieceLevelCap")) {
				levelObject->compositePieceLevelCap = locationItemJSON["compositePieceLevelCap"].asInt();
			}
			if (locationItemJSON.isMember("brainSensorSize")) {
				levelObject->brainSensorSize = locationItemJSON["brainSensorSize"].asInt();
			}
			if (locationItemJSON.isMember("containerRespawnTimer")) {
				levelObject->containerRespawnTimer = locationItemJSON["containerRespawnTimer"].asFloat();
			}
			if (locationItemJSON.isMember("containerStartCount")) {
				levelObject->containerStartCount = locationItemJSON["containerStartCount"].asFloat();
			}
			if (locationItemJSON.isMember("containerCapacity")) {
				levelObject->containerCapacity = locationItemJSON["containerCapacity"].asFloat();
			}


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
	if (leftColor == Colors::BLACK) {
		borderWalls |= wallOnLeft;
	}
	if (rightColor == Colors::BLACK) {
		borderWalls |= wallOnRight;
	}
	if (topColor == Colors::BLACK) {
		borderWalls |= wallOnTop;
	}
	if (bottomColor == Colors::BLACK) {
		borderWalls |= wallOnBottom;
	}

	//build levelObject Here
	if (borderWalls == open)
	{
		levelObject.gameObjectType = "WALL1_OPEN";
	}
	else if (borderWalls == topEnd)
	{
		levelObject.gameObjectType = "WALL1_END";
	}
	else if (borderWalls == bottomEnd)
	{
		levelObject.gameObjectType = "WALL1_END";
		levelObject.angleAdjustment = 180;
	}
	else if (borderWalls == rightEnd)
	{
		levelObject.gameObjectType = "WALL1_END";
		levelObject.angleAdjustment = 90;
	}
	else if (borderWalls == leftEnd)
	{
		levelObject.gameObjectType = "WALL1_END";
		levelObject.angleAdjustment = -90;
	}
	else if (borderWalls == topLeftCorner)
	{
		levelObject.gameObjectType = "WALL1_CORNER";
	}
	else if (borderWalls == topRightCorner)
	{
		levelObject.gameObjectType = "WALL1_CORNER";
		levelObject.angleAdjustment = 90;
	}
	else if (borderWalls == bottomLeftCorner)
	{
		levelObject.gameObjectType = "WALL1_CORNER";
		levelObject.angleAdjustment = -90;
	}
	else if (borderWalls == bottomRightCorner)
	{
		levelObject.gameObjectType = "WALL1_CORNER";
		levelObject.angleAdjustment = 180;
	}
	else if (borderWalls == hallHorz)
	{
		levelObject.gameObjectType = "WALL1_HALL";
	}
	else if (borderWalls == hallVert)
	{
		levelObject.gameObjectType = "WALL1_HALL";
		levelObject.angleAdjustment = 90;
	}
	else if (borderWalls == topWall)
	{
		levelObject.gameObjectType = "WALL1_WALL";
	}
	else if (borderWalls == rightWall)
	{
		levelObject.gameObjectType = "WALL1_WALL";
		levelObject.angleAdjustment = 90;
	}
	else if (borderWalls == bottomWall)
	{
		levelObject.gameObjectType = "WALL1_WALL";
		levelObject.angleAdjustment = 180;
	}
	else if (borderWalls == leftWall)
	{
		levelObject.gameObjectType = "WALL1_WALL";
		levelObject.angleAdjustment = -90;
	}
	else if (borderWalls == column)
	{
		levelObject.gameObjectType = "WALL1_COLUMN";
	}

	//Wall color override if exists
	if (m_wallColor.has_value()) {
		levelObject.color = m_wallColor;
	}

	return levelObject;

}

bool LevelManager::_isColorDefinedObject(SDL_Color color)
{

	for (Json::Value colorItemJSON : m_colorDefinedList) {

		SDL_Color definedColor = game->colorMap()->toSDLColor(colorItemJSON["colorKey"].asString());
		if (definedColor == color) {
			return true;
		}
	}

	return false;

}

void LevelManager::_buildNavigationMapItem(GameObject* gameObject, Scene* scene)
{

	NavigationMapItem navigationMapItem{};

	//This is a normal wall
	if (gameObject->hasTrait(TraitTag::impasse) || 
		gameObject->hasTrait(TraitTag::conditional_impasse) || 
		gameObject->hasTrait(TraitTag::complex_impasse))
	{

		//Find this objects shared_ptr so we can store the weak_ptr of it here
		auto gameObjectSharedPtr = scene->getGameObject(gameObject->id()).value();
		auto angle = gameObjectSharedPtr->getAngleInDegrees();

		navigationMapItem.gameObject = gameObjectSharedPtr;

		//If this is impasse, then it will always be so set it now so that the 
		//the navigationComponent will work on the first pass
		if(gameObject->hasTrait(TraitTag::impasse)){
			navigationMapItem.passable = false;
		}

		//Add the initial navigationMapItem to the collection
		scene->addNavigationMapItem(navigationMapItem, 
			(int)gameObject->getOriginalTilePosition().x, 
			(int)gameObject->getOriginalTilePosition().y);

		//Assert check that certain conditional na objects have to be a 0 or 90
		if (gameObject->hasTrait(TraitTag::conditional_impasse) || 
			gameObject->hasTrait(TraitTag::complex_impasse)) {

			assert((angle == 0 || angle == 90) && "A conditional_impasse object needs to be 0 or 90 degrees,(top to bottom or Left to right)");

		}

	}
	else {

		navigationMapItem.passable = true;
		scene->addNavigationMapItem(navigationMapItem,
			(int)gameObject->getOriginalTilePosition().x,
			(int)gameObject->getOriginalTilePosition().y);

	}

}

void LevelManager::_buildLevelObjects(Scene* scene)
{
	LevelObject* levelObject;

	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {

			if (m_levelObjects[x][y].gameObjectType.empty() == false) {

				levelObject = &m_levelObjects[x][y];

				auto gameObject = scene->addGameObject(levelObject->gameObjectType, levelObject->layer,
					(float)x, (float)y, (float)levelObject->angleAdjustment, levelObject->cameraFollow, levelObject->name);

				//Apply color override
				if (levelObject->color.has_value()) {
					gameObject->setColor(levelObject->color.value());
				}

				//Apply disabled override
				if (levelObject->disabledType.has_value()) {
					
					if (levelObject->disabledType.value() == DISABLED_TYPE::RENDER) {
						gameObject->disableRender();
					}
					else if (levelObject->disabledType.value() == DISABLED_TYPE::UPDATE) {
						gameObject->disableUpdate();
					}
					else if (levelObject->disabledType.value() == DISABLED_TYPE::PHYSICS) {
						gameObject->disablePhysics();
					}
					else if (levelObject->disabledType.value() == DISABLED_TYPE::RENDER_AND_PHYSICS) {
						gameObject->disableRender();
						gameObject->disablePhysics();
					}
					else if (levelObject->disabledType.value() == DISABLED_TYPE::RENDER_AND_UPDATE) {
						gameObject->disableRender();
						gameObject->disableUpdate();
					}
					else if (levelObject->disabledType.value() == DISABLED_TYPE::PHYICS_AND_UPDATE) {
						gameObject->disablePhysics();
						gameObject->disableUpdate();
					}

				}

				//Apply override weapon color if exists
				if (levelObject->weaponColor.has_value()) {
					gameObject->setWeaponColor(levelObject->weaponColor.value());
				}

				//Apply override weapon force if exists
				if (levelObject->weaponForce.has_value()) {
					gameObject->setWeaponForce(levelObject->weaponForce.value());
				}

				//Apply override vitality levelCap if exists
				if (levelObject->compositePieceLevelCap.has_value()) {
					gameObject->setCompositePieceLevelCap(levelObject->compositePieceLevelCap.value());
				}

				//Apply override brain sensor size if exists
				if (levelObject->brainSensorSize.has_value()) {
					gameObject->setBrainSensorSize(levelObject->brainSensorSize.value());
				}

				//Apply override container respawnTimer if exists
				if (levelObject->containerRespawnTimer.has_value()) {
					gameObject->setContainerResapwnTimer(levelObject->containerRespawnTimer.value());
				}

				//Apply override container startCount if exists
				if (levelObject->containerStartCount.has_value()) {
					gameObject->setContainerStartCount(levelObject->containerStartCount.value());
				}

				//Apply override container capacity if exists
				if (levelObject->containerCapacity.has_value()) {
					gameObject->setContainerCapacity(levelObject->containerCapacity.value());
				}

				//Build the navigation map item for this object
				_buildNavigationMapItem(gameObject, scene);

			}

		}
	}
}

void LevelManager::_buildTiledLayers(Scene* scene)
{


	for (auto& tiledLayer : m_tiledLayerDefinitions) {


		//Calculate the number of background tiles horizontal and vertical
		//Add 1 in case
		auto widthCount = (m_tileWidth * m_width) / tiledLayer.second.tileSize.x;
		auto heightCount = (m_tileHeight * m_height) / tiledLayer.second.tileSize.y;

		auto widthRemainder = (m_tileWidth * m_width) % tiledLayer.second.tileSize.x;
		auto heightRemainder = (m_tileHeight * m_height) % tiledLayer.second.tileSize.y;

		//If it did not divide evenly then we need to add 1 to cover the screen
		if (widthRemainder > 0) {
			widthCount += 1;
		}
		if (heightRemainder > 0) {
			heightCount += 1;
		}

		//adjust the X and Y map position based on the size of our background tiles 
		//and the size of all other tiles in the game
		auto adjustX = tiledLayer.second.tileSize.x / m_tileWidth;
		auto adjustY = tiledLayer.second.tileSize.y / m_tileHeight;

		for (auto y = 0; y < heightCount; y++) {
			for (auto x = 0; x < widthCount; x++) {

				//adjust the X and Y map position based on the size of our background tiles
				auto newBackgroundObject = 
					scene->addGameObject(
						tiledLayer.second.tiledObjectId, GameLayer::BACKGROUND_1, (float)x * adjustX, (float)y * adjustY, (float)0);

			}

		}
	}

}

void LevelManager::_buildLevelObjectives(Scene* scene)
{


	if (m_levelDefinition.isMember("objectives")) {

		for (Json::Value itrObjective : m_levelDefinition["objectives"])
		{
			Objective objective{};
			objective.id = game->enumMap()->toEnum(itrObjective["id"].asString());
			objective.targetValue = itrObjective["targetValue"].asFloat();
			scene->addLevelObjective(objective);

		}

	}

}

void LevelManager::_buildLevelStatusItems(std::string levelId)
{

	//All statusItems should already exist in the contextManager
	//We need to initilaize them here since each level can have different min and max, etc for each status item
	if (m_levelDefinition.isMember("statusItems")) {

		game->contextMananger()->clearStatusItems();

		for (Json::Value itrObjective : m_levelDefinition["statusItems"])
		{
			
			int statusItemId = game->enumMap()->toEnum(itrObjective["id"].asString());
			StatusItem statusItem(statusItemId);
			statusItem.setValue(itrObjective["initialValue"].asFloat());
			statusItem.setInitialValue(itrObjective["initialValue"].asFloat());
			statusItem.setMinValue(itrObjective["minValue"].asFloat());
			statusItem.setMaxValue(itrObjective["maxValue"].asFloat());
			game->contextMananger()->addStatusItem(statusItemId, statusItem);

		}

	}

}

void LevelManager::_buildLevelTriggers(Scene* scene)
{

	//Get all trigger items
	if (m_levelDefinition.isMember("levelTriggers")) {

		for (Json::Value itrTrigger : m_levelDefinition["levelTriggers"])
		{
			//Get the name of the class to be used as the action as a string
			std::string triggerId = itrTrigger["triggerClass"].asString();
			std::shared_ptr<Trigger> trigger = game->triggerFactory()->create(triggerId);
			scene->addLevelTrigger(std::move(trigger));

		}

	}

}
