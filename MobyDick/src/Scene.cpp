#include "Scene.h"


#include "EnumMaps.h"

#include "GameObjectManager.h"
#include "game.h"



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
	auto inputControlMode = EnumMap::instance().toEnum(definitionJSON["inputControlMode"].asString());
	m_inputControlMode = inputControlMode;
	setInputControlMode(inputControlMode);

	//GameObjects that are defined at the scene level, not a level built scene
	_buildSceneGameObjects(definitionJSON);

	//Tags
	for (Json::Value itrTag : definitionJSON["tags"]) {

		std::size_t tag = EnumMap::instance().toEnum(itrTag.asString());
		m_sceneTags.set(tag);
	}

	//Keycode actions
	for (Json::Value keyActionJSON : definitionJSON["keyActions"]) {

		auto keyCode = EnumMap::instance().toEnum(keyActionJSON["keyCode"].asString());

		auto action = EnumMap::instance().toEnum(keyActionJSON["sceneAction"]["action"].asString());
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

}

Scene::~Scene()
{

	clear();

}

void Scene::loadLevel(std::string levelId)
{

	game->_displayLoadingMsg();

	reset();

	//Build all of the gameObjects from the level definition
	LevelManager::instance().load(levelId, this);

	//Run GameObject code that requires ALL gameObjects to be created first, for interdependency logic
	_processGameObjectInterdependecies();

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

	//Clear everything
	m_objectPoolManager.clear();

	for (int x = 0; x < MAX_GAMEOBJECT_LAYERS; x++)
	{
		m_gameObjects[x].clear();
	}

	LevelManager::instance().clearTriggers();

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

	// Remove all objects that should be removed in first pass
	for (auto& gameObjects : m_gameObjects)	{
		
		auto it = gameObjects.begin();
		while(it != gameObjects.end()){

			if (it->get()->removeFromWorld() == true) {

				it->get()->reset();
				it = gameObjects.erase(it);
			}
			else {
				++it;
			}
		}

		gameObjects.shrink_to_fit();

	}

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

	//Level Manager update to handle level specific events
	LevelManager::instance().update(this);
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

	auto& gameObject = m_gameObjects[layer].emplace_back(std::make_shared<GameObject>(gameObjectId, xMapPos, yMapPos, angle, this, cameraFollow, name));

	return gameObject.get();

}

GameObject* Scene::addGameObject(std::string gameObjectId, int layer, PositionAlignment windowPosition, float angle, bool cameraFollow)
{
	float xMapPos{};
	float yMapPos{};
	Json::Value definitionJSON;

	//Build components
	if (gameObjectId.rfind("DEBUG_", 0) != 0)
	{
		definitionJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();
	}
	else
	{
		definitionJSON = GameObjectManager::instance().getDefinition("DEBUG_ITEM")->definitionJSON();
	}


	auto& gameObject = m_gameObjects[layer].emplace_back(std::make_shared<GameObject>(gameObjectId, -5, -5, angle, this, cameraFollow));

	auto objectWidth = gameObject->getSize().x;
	auto objectHeight = gameObject->getSize().y;

	if (windowPosition == PositionAlignment::CENTER) {

		xMapPos = (float)GameConfig::instance().windowWidth() / 2;
		yMapPos = (float)GameConfig::instance().windowHeight() / 2;

	}
	else if (windowPosition == PositionAlignment::TOP_CENTER) {

		xMapPos = (float)GameConfig::instance().windowWidth() / 2 ;
		yMapPos = (objectHeight / 2);
	}
	else if (windowPosition == PositionAlignment::TOP_LEFT) {

		xMapPos = (objectWidth / 2);
		yMapPos = (objectHeight / 2);
	}
	else if (windowPosition == PositionAlignment::TOP_RIGHT) {

		xMapPos = (float)(GameConfig::instance().windowWidth() - (objectWidth/2)) ;
		yMapPos = (objectHeight / 2);
	}
	else if (windowPosition == PositionAlignment::CENTER_LEFT) {

		xMapPos = (objectWidth / 2);
		yMapPos = (float)GameConfig::instance().windowHeight() / 2;
	}
	else if (windowPosition == PositionAlignment::CENTER_RIGHT) {

		xMapPos = (float)(GameConfig::instance().windowWidth() - (objectWidth / 2));
		yMapPos = (float)GameConfig::instance().windowHeight() / 2;
	}
	else if (windowPosition == PositionAlignment::BOTTOM_LEFT) {

		xMapPos = (objectWidth / 2);
		yMapPos = (float)(GameConfig::instance().windowHeight() - objectHeight);
	}
	else if (windowPosition == PositionAlignment::BOTTOM_CENTER) {

		xMapPos = (float)(GameConfig::instance().windowWidth()/2 );
		yMapPos = (float)(GameConfig::instance().windowHeight() - objectHeight);
	}
	else if (windowPosition == PositionAlignment::BOTTOM_RIGHT) {

		xMapPos = (float)(GameConfig::instance().windowWidth() - (objectWidth / 2));
		yMapPos = (float)(GameConfig::instance().windowHeight() - objectHeight);
	}

	

	gameObject->setPosition(xMapPos, yMapPos);

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


void Scene::_processGameObjectInterdependecies()
{

	for (auto& layer : m_gameObjects) {

		for (auto& gameObject : layer) {

			gameObject->postInit();

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
	//m_physicsWorld->SetAutoClearForces(true);

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

		auto layer = EnumMap::instance().toEnum(gameObjectJSON["layer"].asString());

		//Determine location
		auto locationJSON = gameObjectJSON["location"];
		if (locationJSON.isMember("windowPosition")) {

			PositionAlignment windowPosition = static_cast<PositionAlignment>(EnumMap::instance().toEnum(gameObjectJSON["location"]["windowPosition"].asString()));
			addGameObject(id, layer, windowPosition, 0);
		}
		else {
			auto locationX = gameObjectJSON["location"]["x"].asFloat();
			auto locationY = gameObjectJSON["location"]["y"].asFloat();
			addGameObject(id, layer, locationX, locationY, 0);
		}
	}
}

GameObject* Scene::getGameObject(std::string name)
{
	std::optional<GameObject*> foundGameObject{};

	for (auto& layer : m_gameObjects) {

		for (auto& gameObject : layer) {

			if (gameObject->name() == name) {

				foundGameObject = gameObject.get();
				break;
			}
		}
	}

	assert(foundGameObject.has_value() && "GameObject wasnt found!");

	return foundGameObject.value();
}

