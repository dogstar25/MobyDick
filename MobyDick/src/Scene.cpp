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
		auto newSceneId = keyActionJSON["sceneAction"]["newSceneId"].asString();

		SceneAction sceneAction = { int(action) , newSceneId };
		addKeyAction(keyCode, sceneAction);
	}

	//Debug Mode
	if (m_hasPhysics && m_physicsConfig.b2DebugDrawMode == true)
	{
		DebugDraw::instance().SetFlags(DebugDraw::e_shapeBit);
		m_physicsWorld->SetDebugDraw(&DebugDraw::instance());
	}

	//Initialize the camera state
	//Camera::instance().init();

}

Scene::~Scene()
{

	clear();

}

void Scene::loadLevel(std::string levelId)
{

	m_currentLevelId = levelId;

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
		assert(false && "We are already at the last level!");
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
	if (m_hasPhysics && m_physicsConfig.b2DebugDrawMode == true)
	{
		DebugDraw::instance().SetFlags(DebugDraw::e_shapeBit);
		m_physicsWorld->SetDebugDraw(&DebugDraw::instance());
	}

}

void Scene::clear()
{

	Camera::instance().init();

	//Clear everything
	m_objectPoolManager.clear();
	m_levelTriggers.clear();

	//Clear the objective contextManager values as well as the objective items stored in the scene
	//for (auto& objective : m_levelObjectives) {

	//	auto& objectiveStatusItem = game->contextMananger()->getStatusItem(objective.contextManagerId);
	//	objectiveStatusItem.setOriginalValue(0);
	//	objectiveStatusItem.setMaxValue(0);
	//	objectiveStatusItem.setValue(0);
	//}
	m_levelObjectives.clear();

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

	// Remove all objects that should be removed in first pass
	_removeFromWorldPass();

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

}

void Scene::render() {

	int gameLayerIndex{0};

	//Render all of the layers
	for (auto& gameLayer : m_gameObjects)
	{

		//USE THE LAYERiNDEX TO DETERMINE IF WE HAVE A PARALLAX and set a new filed on the gameObjects
		//render component
		//_setParallax();

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
	if (m_hasPhysics && m_physicsConfig.b2DebugDrawMode == true)
	{
		m_physicsWorld->DebugDraw();
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

GameObject* Scene::addGameObject(std::string gameObjectId, int layer, float xMapPos, float yMapPos, float angle, bool cameraFollow,std::string name)
{

	auto& gameObject = m_gameObjects[layer].emplace_back(std::make_shared<GameObject>(gameObjectId, xMapPos, yMapPos, angle, this, layer, cameraFollow, name));

	return gameObject.get();

}

GameObject* Scene::addGameObject(std::string gameObjectId, int layer, PositionAlignment windowPosition, float adjustX, float adjustY, float angle, bool cameraFollow)
{

	auto& gameObject = m_gameObjects[layer].emplace_back(std::make_shared<GameObject>(gameObjectId, (float)-5, (float)-5, angle, this, layer, cameraFollow));

	gameObject->setPosition(windowPosition, adjustX, adjustY);

	return gameObject.get();

}

/*
Emplace the new gameObject into the collection and also return a reference ptr to the newly created object as well
*/
GameObject* Scene::addGameObject(std::shared_ptr<GameObject> gameObject, int layer)
{

	gameObject->setParentScene(this);
	auto& gameObjectRef = this->m_gameObjects[layer].emplace_back(gameObject);

	return gameObjectRef.get();

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

	//First delete all player input events from last loop
	//m_PlayerInputEvents.clear();
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
	m_physicsConfig.b2DebugDrawMode = physicsJSON["b2DebugDrawMode"].asBool();

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
	for (Json::Value gameObjectJSON : definitionJSON["gameObjects"]) {

		auto id = gameObjectJSON["gameObjectId"].asString();

		auto layer = game->enumMap()->toEnum(gameObjectJSON["layer"].asString());

		//Determine location
		auto locationJSON = gameObjectJSON["location"];
		if (locationJSON.isMember("windowPosition")) {

			PositionAlignment windowPosition = static_cast<PositionAlignment>(game->enumMap()->toEnum(gameObjectJSON["location"]["windowPosition"].asString()));

			if (locationJSON.isMember("adjust")) {
				auto adjustX = locationJSON["adjust"]["x"].asFloat();
				auto adjustY = locationJSON["adjust"]["y"].asFloat();
				addGameObject(id, layer, windowPosition, adjustX, adjustY);
			}
			else {
				addGameObject(id, layer, windowPosition);
			}
			
		}
		else {
			auto locationX = gameObjectJSON["location"]["x"].asFloat();
			auto locationY = gameObjectJSON["location"]["y"].asFloat();
			addGameObject(id, layer, locationX, locationY, 0);
		}
	}
}

std::optional<std::shared_ptr<GameObject>> Scene::getGameObject(std::string name)
{
	std::optional<std::shared_ptr<GameObject>> foundGameObject{};

	for (auto& layer : m_gameObjects) {

		for (auto& gameObject : layer) {

			if (gameObject->name() == name) {

				return  gameObject;
			}
			else
			{
				foundGameObject = gameObject->getSubGameObject(name);
				if (foundGameObject.has_value()) {
					return foundGameObject;
				}
			}

		}
	}

	assert(foundGameObject.has_value() && "GameObject wasnt found!");

	return foundGameObject;
}

void Scene::_removeFromWorldPass()
{
	//Loop through all layers and remove any gameObject that has been marked to remove
	for (auto& gameObjects : m_gameObjects) {

		auto it = gameObjects.begin();
		while (it != gameObjects.end()) {

			auto test = it->get();

			//Remove gameObject iteself it flagged
			//If it's a physics object then it's probably pooled, but either way we dont want to completely delete it
			//One of the BrainComponents may have stored this objects userData (gameObject raw pointer) and so completely deleting it
			//would compromise those pointers
			if (it->get()->removeFromWorld() == true) {

				if (it->get()->hasTrait(TraitTag::pooled)) {

					it->get()->reset();

				}
			
				it = gameObjects.erase(it);
			}
			else {
				++it;
			}
		}

		gameObjects.shrink_to_fit();

	}

}

void Scene::addLevelObjective(Objective objective)
{
	m_levelObjectives.emplace_back(objective);
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

//SDL_FPoint Scene::calcWindowPosition(int globalPosition)
//{
//	SDL_FPoint globalPoint = {};
//
//	if (globalPosition == PositionAlignment::CENTER) {
//
//		globalPoint.x = (float)round(GameConfig::instance().windowWidth() / game->worldTileWidth() / 2);
//		globalPoint.y = (float)round(GameConfig::instance().windowHeight() / game->worldTileHeight() / 2);
//
//	}
//	else if (globalPosition == PositionAlignment::TOP_CENTER) {
//
//		globalPoint.x = (float)round(GameConfig::instance().windowWidth() / game->worldTileWidth() / 2);
//		globalPoint.y = 0;
//	}
//	else if (globalPosition == PositionAlignment::TOP_LEFT) {
//
//		globalPoint.x = 0;
//		globalPoint.y = 0;
//	}
//	else if (globalPosition == PositionAlignment::TOP_RIGHT) {
//
//		globalPoint.x = (float)round(GameConfig::instance().windowWidth() / game->worldTileWidth() - 5);
//		globalPoint.y = 0;
//	}
//	else if (globalPosition == PositionAlignment::CENTER_LEFT) {
//
//		globalPoint.x = 0;
//		globalPoint.y = (float)round(GameConfig::instance().windowHeight() / game->worldTileHeight() / 2);
//	}
//	else if (globalPosition == PositionAlignment::BOTTOM_LEFT) {
//
//		globalPoint.x = 0;
//		globalPoint.y = (float)round(GameConfig::instance().windowHeight() / game->worldTileHeight());
//	}
//
//	else {
//		/* Need other calcs added*/
//	}
//
//	return globalPoint;
//
//}

