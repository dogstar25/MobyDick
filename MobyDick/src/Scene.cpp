#include "Scene.h"


#include "EnumMap.h"

#include "GameObjectManager.h"
#include "SoundManager.h"
#include "game.h"
#include "LevelManager.h"

extern std::unique_ptr<Game> game;

Scene::Scene(std::string sceneId)
{

	//Get the scene definition
	Json::Value definitionJSON = SceneManager::instance().getDefinition(sceneId);
	m_id = sceneId;

	//Start the scene in run mode
	m_state = SceneState::RUN;

	//Physics if its needed for the scene
	if (definitionJSON.isMember("physics")) {
		m_hasPhysics = true;
		Json::Value physicsJSON = definitionJSON["physics"];
		_buildPhysicsWorld(physicsJSON);
	}

	//Allocate the arrays for all of the gameObjects
	auto maxObjects = definitionJSON["maxObjects"].asInt();
	for (auto& gameLayer : m_gameObjects)
	{
		gameLayer.reserve(maxObjects);
	}

	//Build ObjectPoolManager
	m_objectPoolManager.init(definitionJSON, this);

	//Set the mouse mode
	auto inputControlMode = game->enumMap()->toEnum(definitionJSON["inputControlMode"].asString());
	m_inputControlMode = inputControlMode;
	setInputControlMode(inputControlMode);

	//GameObjects that are defined at the scene level, not a level built scene
	_buildSceneGameObjects(definitionJSON);

	//Tags
	for (Json::Value itrTag : definitionJSON["tags"]) {

		std::size_t tag = game->enumMap()->toEnum(itrTag.asString());
		m_sceneTags.set(tag);
	}

	//Keycode actions
	for (Json::Value keyActionJSON : definitionJSON["keyActions"]) {

		auto keyCode = game->enumMap()->toEnum(keyActionJSON["keyCode"].asString());

		auto action = game->enumMap()->toEnum(keyActionJSON["sceneAction"]["action"].asString());
		auto actionId = keyActionJSON["sceneAction"]["actionId"].asString();

		SceneAction sceneAction = { int(action) , actionId };
		addKeyAction(keyCode, sceneAction);
	}

	//Debug Mode
	if (m_hasPhysics == true)
	{
		DebugDraw::instance().SetFlags(DebugDraw::e_shapeBit);
		m_physicsWorld->SetDebugDraw(&DebugDraw::instance());
	}

	//If there is background music specified then play it
	if (definitionJSON.isMember("backgroundMusicId")) {

		std::string backGroundMusicId = definitionJSON["backgroundMusicId"].asString();
		SoundManager::instance().playMusic(backGroundMusicId, -1);
	}

}

Scene::~Scene()
{

	clear();

}

void Scene::setDebugSetting(int setting) {

	m_sceneDebugSettings.flip(setting);

	//If this is the debug navigation grid then reset it
	if (setting == DebugSceneSettings::SHOW_NAVIGATION_DEBUG_MAP) {
		resetGridDisplay();
	}


}

bool Scene::isDebugSetting(int setting)
{

	return m_sceneDebugSettings.test(setting);

}

void Scene::loadLevel(std::string levelId)
{

	m_currentLevelId = levelId;

	//Stop music from previous level
	SoundManager::instance().stopMusic();

	game->_displayLoadingMsg();

	reset();

	//Build all of the gameObjects from the level definition
	LevelManager::instance().loadLevel(levelId, this);

	//Run GameObject code that requires ALL gameObjects to be created first, for interdependency logic
	_processGameObjectInterdependecies();

}

void Scene::loadNextLevel()
{

	std::optional<std::string> nextLevelId = getNextLevel();
	if (nextLevelId.has_value()) {
		loadLevel(nextLevelId.value());
	}
	else {
		
		util::sendSceneEvent(SCENE_ACTION_ADD_AND_PAUSE, "SCENE_YOU_WIN");

	}

}

std::optional<std::string> Scene::getNextLevel()
{

	return LevelManager::instance().getNextLevelId(m_currentLevelId);

}

void Scene::loadCurrentLevel()
{

	loadLevel(m_currentLevelId);

}

std::optional<SceneAction> Scene::getkeycodeAction(SDL_Keycode keycode) {
	if (m_sceneKeyActions.find(keycode) != m_sceneKeyActions.end()) {
		return m_sceneKeyActions.at(keycode);
	}
	else {
		return std::nullopt;
	}
}

void Scene::reset()
{

	clear();

	//Rebuild everything
	game->_displayLoadingMsg();

	Json::Value sceneJSON = SceneManager::instance().getDefinition(m_id);

	if (sceneJSON.isMember("physics")) {
		m_hasPhysics = true;
		Json::Value physicsJSON = sceneJSON["physics"];
		_buildPhysicsWorld(physicsJSON);
	}

	m_objectPoolManager.init(sceneJSON, this);

	//GameObjects that are defined at the scene level, not a level built scene
	_buildSceneGameObjects(sceneJSON);

	//Debug Mode
	if (m_hasPhysics == true)
	{
		DebugDraw::instance().SetFlags(DebugDraw::e_shapeBit);
		m_physicsWorld->SetDebugDraw(&DebugDraw::instance());
	}

	//DebugSettings
	m_sceneDebugSettings.reset();

}

void Scene::clear()
{

	Camera::instance().init();

	//Clear everything
	m_objectPoolManager.clear();
	m_levelTriggers.clear();

	m_levelObjectives.clear();
	m_gameObjectLookup.clear();
	m_navigationMap.clear();

	for (int x = 0; x < MAX_GAMEOBJECT_LAYERS; x++)
	{
		m_gameObjects[x].clear();
	}

	if (m_hasPhysics) {
		delete m_physicsWorld;
	}

}

void Scene::update() {

	//Direct the scne if it has a cutScene assigned
	if (m_cutScene.has_value() == true) {
		direct();
	}

	Camera::instance().update();
	SoundManager::instance().update();

	if (hasPhysics()) {
		stepB2PhysicsWorld();
	}

	//Update each gameObject in all layers
	for (auto& gameObjects : m_gameObjects)
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->update();
		}
	}

	//Clear all events
	SceneManager::instance().playerInputEvents().clear();

	//Check all level triggers
	for (const auto& trigger : m_levelTriggers) {

		if (trigger->hasMetCriteria(this)) {
			trigger->execute();
		}
	}

	// Remove all objects that should be removed in first pass
	_removeFromWorldPass();

	//Update the navigationMap
	m_navigationMapChanged = _updateNavigationMap();


	//If the Debug display navigation grid is turned on then build it
	if (isDebugSetting(DebugSceneSettings::SHOW_NAVIGATION_DEBUG_MAP)) {
		_showNavigationMap();
	}

}

void Scene::render() {

	int gameLayerIndex{0};

	//Render all of the layers
	for (auto& gameLayer : m_gameObjects)
	{

		//Render all of the GameObjects in this layer
		for (auto& gameObject : gameLayer)
		{
			gameObject->render();
		}

		//Render any primitive object for this layer (lines and single pixels/points)
		game->renderer()->renderPrimitives(gameLayerIndex);

		gameLayerIndex++;

	}
	
	//DebugDraw
	if (m_hasPhysics && isDebugSetting(DebugSceneSettings::SHOW_PHYSICS_DEBUG) == true)
	{
		m_physicsWorld->DebugDraw();
	}

}

void Scene::resetGridDisplay()
{

	for (const auto& gameObject : m_gameObjects[GameLayer::GRID_DISPLAY]) {

		gameObject->disableRender();

	}


}

void Scene::direct()
{

	m_cutScene.value()->run(this);

}

void Scene::setCutScene(std::shared_ptr<CutScene>cutScene)
{

	m_cutScene = cutScene;

}

void Scene::deleteCutScene()
{

	m_cutScene.reset();

}

GameObject* Scene::addGameObject(std::string gameObjectType, int layer, float xMapPos, float yMapPos, float angle, bool cameraFollow,std::string name)
{

	auto& gameObject = m_gameObjects[layer].emplace_back(std::make_shared<GameObject>(gameObjectType, xMapPos, yMapPos, angle, this, layer, cameraFollow, name));

	//Add index
	addGameObjectIndex(gameObject);

	return gameObject.get();

}

GameObject* Scene::addGameObject(std::string gameObjectType, int layer, PositionAlignment windowPosition, float adjustX, float adjustY, float angle, bool cameraFollow)
{

	auto& gameObject = m_gameObjects[layer].emplace_back(std::make_shared<GameObject>(gameObjectType, (float)-5, (float)-5, angle, this, layer, cameraFollow));
	gameObject->setPosition(windowPosition, adjustX, adjustY);

	//Set the window position override
	gameObject->setWindowRelativePosition( windowPosition, adjustX, adjustY);

	//Add index 
	addGameObjectIndex(gameObject);

	return gameObject.get();

}

/*
Emplace the new gameObject into the collection and also return a reference ptr to the newly created object as well
*/
void Scene::addGameObject(std::shared_ptr<GameObject> gameObject, int layer)
{

	gameObject->setParentScene(this);
	this->m_gameObjects[layer].push_back(gameObject);

	//Add index 
	addGameObjectIndex(gameObject);

	return;;

}

void Scene::addGameObjectIndex(std::shared_ptr<GameObject> gameObject)
{

	const auto gameObjectPair = m_gameObjectLookup.emplace(std::pair<std::string, std::shared_ptr<GameObject>>(gameObject->id(), gameObject));

	return;

}



void Scene::addKeyAction(SDL_Keycode keyCode, SceneAction sceneAction)
{
	m_sceneKeyActions.emplace(keyCode, sceneAction);
}

void Scene::applyCurrentControlMode()
{

	setInputControlMode(m_inputControlMode);

}

std::optional<Parallax> Scene::getParallax(int layer)
{

	if (m_parallaxRates.find(layer) != m_parallaxRates.end()) {
		return m_parallaxRates.at(layer);
	}
	else {
		return std::nullopt;
	}

}


void Scene::clearEvents()
{

}

void Scene::setInputControlMode(int inputControlMode)
{

	m_inputControlMode = inputControlMode;

	if (inputControlMode == CONTROL_MODE_PLAY) {
		SDL_ShowCursor(false);
		SDL_SetRelativeMouseMode(SDL_TRUE);
		game->IMGuiControlled = false;
	}
	else if (inputControlMode == CONTROL_MODE_SELECT) {
		SDL_ShowCursor(true);
		SDL_SetRelativeMouseMode(SDL_FALSE);
		game->IMGuiControlled = false;
	}
	else if (inputControlMode == CONTROL_MODE_IMGUI) {
		SDL_ShowCursor(true);
		SDL_SetRelativeMouseMode(SDL_FALSE);
		game->IMGuiControlled = true;
	}

}


void Scene::_processGameObjectInterdependecies()
{

	for (auto& layer : m_gameObjects) {

		for (auto& gameObject : layer) {

			gameObject->postInit();

			//Now that all gameobjects are created we can set the shared pointer for the object to follow for the camera
			if (gameObject->name() == Camera::instance().getFollowMeName() && !Camera::instance().getFollowMeObject()) {
				Camera::instance().setFollowMe(gameObject);
			}

		}

	}

}

void Scene::_buildPhysicsWorld(Json::Value physicsJSON)
{

 	m_physicsConfig.gravity.Set(physicsJSON["gravity"]["x"].asFloat(), physicsJSON["gravity"]["y"].asFloat());
	m_physicsConfig.timeStep = physicsJSON["timeStep"].asFloat();
	m_physicsConfig.velocityIterations = physicsJSON["velocityIterations"].asInt();
	m_physicsConfig.positionIterations = physicsJSON["positionIterations"].asInt();
	//m_physicsConfig.b2DebugDrawMode = physicsJSON["b2DebugDrawMode"].asBool();

	//Build the box2d physics world
	m_physicsWorld = new b2World(m_physicsConfig.gravity);

	//Add a collision contact listener and filter for box2d callbacks
	m_physicsWorld->SetContactFilter(game->contactFilter().get());
	m_physicsWorld->SetContactListener(game->contactListener().get());

}

void _updatePhysics(b2World* physicsWorld)
{
	//Update ALL physics object states
	physicsWorld->Step(.016, 6, 2);
}

void Scene::_buildSceneGameObjects(Json::Value definitionJSON)
{
	GameObject* gameObject{};

	for (Json::Value gameObjectJSON : definitionJSON["gameObjects"]) {

		std::string gameObjectType = gameObjectJSON["gameObjectType"].asString();
		assert(!gameObjectType.empty() && "Empty GameObejectType in Scene definition for scene ");

		auto layer = game->enumMap()->toEnum(gameObjectJSON["layer"].asString());

		//Determine location
		auto locationJSON = gameObjectJSON["location"];
		if (locationJSON.isMember("windowPosition")) {

			PositionAlignment windowPosition = static_cast<PositionAlignment>(game->enumMap()->toEnum(gameObjectJSON["location"]["windowPosition"].asString()));

			if (locationJSON.isMember("adjust")) {
				auto adjustX = locationJSON["adjust"]["x"].asFloat();
				auto adjustY = locationJSON["adjust"]["y"].asFloat();
				gameObject = addGameObject(gameObjectType, layer, windowPosition, adjustX, adjustY);
			}
			else {
				gameObject = addGameObject(gameObjectType, layer, windowPosition);
			}
			
		}
		else {
			auto locationX = gameObjectJSON["location"]["x"].asFloat();
			auto locationY = gameObjectJSON["location"]["y"].asFloat();
			gameObject = addGameObject(gameObjectType, layer, locationX, locationY, 0);
		}

		gameObject->postInit();
	}
}

std::optional<std::shared_ptr<GameObject>> Scene::getGameObject(std::string id)
{
	std::optional<std::shared_ptr<GameObject>> foundGameObject{};


	auto search = m_gameObjectLookup.find(id);
	if (search != m_gameObjectLookup.end()) {
		foundGameObject = search->second.lock();
	}

	return foundGameObject;
}

std::vector<std::shared_ptr<GameObject>> Scene::getGameObjectsByName(std::string name)
{
	std::vector<std::shared_ptr<GameObject>> foundGameObjects;

	auto it = m_gameObjectLookup.begin();
	while (it != m_gameObjectLookup.end()) {

		if (it->second.lock()->name() == name) {
			foundGameObjects.push_back(it->second.lock());
		}

		++it;
	}

	return foundGameObjects;
}

std::optional<std::shared_ptr<GameObject>> Scene::getFirstGameObjectByName(std::string name)
{
	std::optional<std::shared_ptr<GameObject>> foundGameObject{};

	auto it = m_gameObjectLookup.begin();
	while (it != m_gameObjectLookup.end()) {

		if (it->second.lock()->name() == name) {
			foundGameObject = it->second.lock();
			break;
		}

		++it;
	}

	return foundGameObject;
}

std::vector<std::shared_ptr<GameObject>> Scene::getGameObjectsByTrait(int trait)
{
	std::vector<std::shared_ptr<GameObject>> foundGameObjects;

	auto it = m_gameObjectLookup.begin();
	while (it != m_gameObjectLookup.end()) {

		if (it->second.lock()->hasTrait(trait)) {
			foundGameObjects.push_back(it->second.lock());
		}

		++it;
	}

	return foundGameObjects;
}

std::optional<std::shared_ptr<GameObject>> Scene::getFirstGameObjectByTrait(int trait)
{
	std::optional<std::shared_ptr<GameObject>> foundGameObject{};

	auto it = m_gameObjectLookup.begin();
	while (it != m_gameObjectLookup.end()) {

		if (it->second.lock()->hasTrait(trait)) {
			foundGameObject = it->second.lock();
			break;
		}

		++it;
	}

	return foundGameObject;
}

void Scene::deleteIndex(std::string gameObjectKey)
{
	//std::cout << "Erased from lookup map" << gameObjectKey << std::endl;
	m_gameObjectLookup.erase(gameObjectKey);
	
}


void Scene::resetTrigger(std::string triggerName)
{

	for (auto trigger : m_levelTriggers) {

		if (trigger->getName() == triggerName) {

			trigger->reset();

		}
	}

}


void Scene::_removeFromWorldPass()
{
	//Loop through all layers and remove any gameObject that has been marked to remove
	for (auto& gameObjects : m_gameObjects) {

		auto it = gameObjects.begin();
		while (it != gameObjects.end()) {

			//Remove gameObject iteself if flagged
			if (it->get()->removeFromWorld() == true) {

				if (it->get()->hasTrait(TraitTag::pooled)) {

					it->get()->reset();

				}

				//std::cout << "Erased from Main collection " << it->get()->name() << std::endl;
				it = gameObjects.erase(it);
				
			}
			else {
				++it;
			}
		}

		gameObjects.shrink_to_fit();

	}

	//Also loop through entire lookup map and delete any expired weak pointers.
	//The strong pointers of the various gameObjects, some of which are dependents inside of other objects,
	//may have been deleted and their weak_ptr reference should be cleaned up in the lookup table also
	for (auto it = m_gameObjectLookup.cbegin(); it != m_gameObjectLookup.cend();)
	{
		if (it->second.expired() == true)
		{
			it = m_gameObjectLookup.erase(it);
		}
		else
		{
			++it;
		}
	}

}

void Scene::addLevelObjective(Objective objective)
{
	m_levelObjectives.emplace_back(objective);
}

void Scene::addNavigationMapItem(NavigationMapItem& navigationMapItem, int x, int y)
{
	m_navigationMap[x][y] = navigationMapItem;
}


void Scene::setNavigationMapArraySize(int width, int height)
{
	m_navigationMap.resize(width, std::vector<NavigationMapItem>(height));

}

void Scene::addLevelTrigger(std::shared_ptr<Trigger> trigger)
{
	m_levelTriggers.emplace_back(trigger);
}


void Scene::addParallaxItem(Parallax& parallaxItem)
{
	//we want to store these by index so they are retrieved on the render quickly
	m_parallaxRates.emplace(parallaxItem.layer, parallaxItem);

}

void Scene::updateGridDisplay(int xPos, int yPos, int operation, SDL_Color color)
{
	//ToDo:This can be changed to use multithreasing
	//use  std::foreach(std::execution::par, m_gameObjects[GameLayer::GRID_DISPLAY.begin, m_gameObjects[GameLayer::GRID_DISPLAY.end, 
    //[this](const std::shared_ptr<GameObject>)
	//{
	// put below code here
	//});

	for (const auto& gameObject : m_gameObjects[GameLayer::GRID_DISPLAY]) {

		int x = (int)gameObject->getOriginalTilePosition().x;
		int y = (int)gameObject->getOriginalTilePosition().y;

		if (x == xPos && y == yPos) {

			if (operation == TURN_OFF) {
				gameObject->disableRender();
			}
			else{
				gameObject->setColor(color);
				gameObject->enableRender();
			}

		}

	}

}

bool Scene::_updateNavigationMap()
{

	bool aValueChanged{};

	for (auto x = 0; x <  m_navigationMap.size(); ++x)
	{
		for (auto y = 0; y < m_navigationMap[x].size(); ++y)
		{


			auto& navItem = m_navigationMap[x][y];

			//If this gameObject hasnt been deleted elsewhere somehow the get the gameObject representing 
			// this navigation map grid location
			if (navItem.gameObject.has_value() && navItem.gameObject->expired() == false) {

				const auto& gameObject = navItem.gameObject->lock();

				//Get the gameobject and its width in tiles
				int widthInTiles = int(std::round(gameObject->getSize().x / LevelManager::instance().m_tileWidth));

				//Plain impassable
				if (gameObject->hasTrait(TraitTag::impasse)) {
					if (m_navigationMap[x][y].passable != false) {
						aValueChanged = true;
					}
					m_navigationMap[x][y].passable = false;
				}
				else if (gameObject->hasTrait(TraitTag::conditional_impasse)) {


					//if we are at 0 angle then we need to set the next "widthInTiles" tiles horizontally to impassable
					if ((int)gameObject->getAngleInDegrees() == 0) {

						if (gameObject->physicsDisabled() == true) {

							//set the next tiles to the right as no-impasse
							for (auto i = 0; i < widthInTiles; i++) {
								if ((x + i) < LevelManager::instance().m_width - 1) {

									if (m_navigationMap[x][y].passable != true) {
										aValueChanged = true;
									}

									m_navigationMap[x + i][y].passable = true;
								}
							}
						}
						else {

							//set the next tiles to the right as no-impasse
							for (auto i = 0; i < widthInTiles; i++) {
								if ((x + i) < LevelManager::instance().m_width - 1) {

									if (m_navigationMap[x][y].passable != false) {
										aValueChanged = true;
									}

									m_navigationMap[x + i][y].passable = false;
								}
							}

						}

					}
					//if we are at 90 angle then we need to set the next "widthInTiles" tiles vertically to impassable
					else if ((int)gameObject->getAngleInDegrees() == 90) {

						if (gameObject->physicsDisabled() == true) {

							//set the next tiles to the right as no-impasse
							for (auto i = 0; i < widthInTiles; i++) {
								if ((y + i) < LevelManager::instance().m_height - 1) {

									if (m_navigationMap[x][y].passable != true) {
										aValueChanged = true;
									}

									m_navigationMap[x][y + i].passable = true;
								}
							}
						}
						else {

							//set the next tiles to the right as no-impasse
							for (auto i = 0; i < widthInTiles; i++) {
								if ((y + i) < LevelManager::instance().m_height - 1) {

									if (m_navigationMap[x][y].passable != false) {
										aValueChanged = true;
									}

									m_navigationMap[x][y + i].passable = false;
								}
							}

						}

					}

				}
				//This is a composite object that needs a different type of check for passable			
				else if (gameObject->hasTrait(TraitTag::complex_impasse)) {


					//if we are at 0 angle then we need to set the next "widthInTiles" tiles to impassable
					if ((int)navItem.gameObject->lock()->getAngleInDegrees() == 0) {

						//Has the composite object been destroyed
						if (gameObject->isCompositeEmpty() == true) {

							//Set the next "widthInTiles" tiles vertically to passable
							for (auto i = 0; i < widthInTiles; i++) {
								if ((x + i) < LevelManager::instance().m_width - 1) {

									if (m_navigationMap[x][y].passable != true) {
										aValueChanged = true;
									}

									m_navigationMap[x + i][y].passable = true;
								}
							}
						}
						else {

							//Set the next "widthInTiles" tiles vertically to impassable
							for (auto i = 0; i < widthInTiles; i++) {
								if ((x + i) < LevelManager::instance().m_width - 1) {

									if (m_navigationMap[x][y].passable != false) {
										aValueChanged = true;
									}

									m_navigationMap[x + i][y].passable = false;
								}
							}

						}

					}
					//if we are at 0 angle then we need to set the next "widthInTiles" tiles to impassable
					else if (gameObject->getAngleInDegrees() == 90) {

						if (gameObject->isCompositeEmpty() == true) {

							//set the next tiles to the right as no-impasse
							for (auto i = 0; i < widthInTiles; i++) {
								if ((y + i) < LevelManager::instance().m_height - 1) {

									if (m_navigationMap[x][y].passable != true) {
										aValueChanged = true;
									}

									m_navigationMap[x][y + i].passable = true;
								}
							}
						}
						else {

							//set the next tiles to the right as no-impasse
							for (auto i = 0; i < widthInTiles; i++) {
								if ((y + i) < LevelManager::instance().m_height - 1) {

									if (m_navigationMap[x][y].passable != false) {
										aValueChanged = true;
									}

									m_navigationMap[x][y + i].passable = false;
								}
							}

						}

					}

				}
			}
		}
	}

	return aValueChanged;

}

void Scene::_showNavigationMap()
{

	for (const auto& gameObject : m_gameObjects[GameLayer::GRID_DISPLAY]) {

		//Get the x,y object from the navigation map
		int x = (int)gameObject->getOriginalTilePosition().x;
		int y = (int)gameObject->getOriginalTilePosition().y;

		if (m_navigationMap[x][y].passable == false) {

			gameObject->enableRender();
				
		}
		else {
			gameObject->disableRender();
		}


	}

}

