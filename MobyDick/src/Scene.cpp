#include "Scene.h"

#include <cassert>

#include "Level.h"
#include "Camera.h"
#include "game.h"
#include "EnumMaps.h"

Scene::Scene(std::string sceneId)
{

	//Get the scene definition
	Json::Value definitionJSON = SceneManager::instance().getDefinition(sceneId);

	m_id = sceneId;
	m_state = SceneState::RUN;

	//Allocate the arrays for all of the gameObjects
	auto maxObjects = definitionJSON["maxObjects"].asInt();
	for (auto& gameLayer : m_gameObjects)
	{
		gameLayer.reserve(maxObjects);
	}

	//Set the mouse mode
	auto inputControlMode = EnumMap::instance().toEnum(definitionJSON["inputControlMode"].asString());
	m_inputControlMode = inputControlMode;

	//Load the First level - ToDo: need level managing implemented somehow
	auto levelId = definitionJSON["firstLevel"].asString();
	if (levelId.empty() == false) {

		Level::instance().load(levelId, this);
	}

	//Tags
	for (Json::Value itrTag : definitionJSON["tags"]) {

		std::size_t tag = EnumMap::instance().toEnum(itrTag.asString());
		m_sceneTags.set(tag);
	}

	//GameOBjects - ones that are not being handled in the level blueprint
	for (Json::Value gameObjectJSON : definitionJSON["gameObjects"]) {

		auto id = gameObjectJSON["gameObjectId"].asString();

		auto layer = EnumMap::instance().toEnum(gameObjectJSON["layer"].asString());

		//Determine location
		auto locationJSON = gameObjectJSON["location"];
		if (locationJSON.isMember("windowPosition")) {

			auto windowPosition = EnumMap::instance().toEnum(gameObjectJSON["location"]["windowPosition"].asString());
			SDL_FPoint location = calcWindowPosition(windowPosition);
			addGameObject(id, layer, location.x, location.y, 0);
		}
		else {
			auto locationX = gameObjectJSON["location"]["x"].asFloat();
			auto locationY = gameObjectJSON["location"]["y"].asFloat();
			addGameObject(id, layer, locationX, locationY, 0);
		}
	}

	//Keycode actions
	for (Json::Value keyActionJSON : definitionJSON["keyActions"]) {

		auto keyCode = EnumMap::instance().toEnum(keyActionJSON["keyCode"].asString());

		auto action = EnumMap::instance().toEnum(keyActionJSON["sceneAction"]["action"].asString());
		auto newSceneId = keyActionJSON["sceneAction"]["newSceneId"].asString();

		SceneAction sceneAction = { int(action) , newSceneId };
		addKeyAction(keyCode, sceneAction);
	}

	//Run GameObject code that requires ALL gameObjects to be created first, for interdependency logic
	//processGameObjectInterdependecies();


}


Scene::~Scene()
{

	/*for (int x = 0; x < MAX_GAMEOBJECT_LAYERS; x++)
	{
		m_gameObjects[x].clear();
	}*/

	//Delete box2d world - should delete all bodies and fixtures within
	//delete m_physicsWorld;

	

}


void Scene::run()
{

	////Capture the amount of time that has passed since last loop and accumulate time for both
	////the FPS calculation and the game loop timer
	//Clock::instance().update();

	////Only update and render if we have passed the 60 fps time passage
	//if (Clock::instance().hasMetGameLoopSpeed())
	//{
	//	//Handle updating objects positions and physics
	//	if (m_state != SceneState::PAUSE) {
	//		update();
	//	}

	//	//render everything
	//	render();

	//	//Increment frame counter and calculate FPS and reset the gameloop timer
	//	Clock::instance().calcFps();

	//}

}

void Scene::update() {


	Camera::instance().update();

	// Remove all objects that should be removed in first pass
	for (auto& gameObjects : m_gameObjects)
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			if (gameObjects[i]->removeFromWorld())
			{
				gameObjects[i]->reset();//not every object is pooled so need a different spot to handle this
				gameObjects.erase(gameObjects.begin() + i);
			}
		}

		//gameObjects.shrink_to_fit();

	}


	//Update each gameObject in all layers
	for (auto& gameObjects : m_gameObjects)
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{
			//assert(gameObject != nullptr && "GameObject is null");
			gameObjects[i]->update();
		}
	}

	//DebugPanel::instance().addItem("Test", util::generateRandomNumber(1,10000), 8);

	//Clear all events
	SceneManager::instance().playerInputEvents().clear();

	//Update ALL physics object states
	Game::instance().stepB2PhysicsWorld();

}



void Scene::render() {

	////Clear the graphics display
	//Renderer::instance().clear();

	//Render all of the game objects
	int i = 0;
	for (auto& gameLayer : m_gameObjects)
	{
		//Update normal game objects
		for (auto& gameObject : gameLayer)
		{
			i++;
			gameObject->render();
		}
	}

	
	//DebugDraw
	if (GameConfig::instance().b2DebugDrawMode() == true)
	{
		Game::instance().physicsWorld()->DrawDebugData();
	}

	////Push all drawn things to the graphics display
	//Renderer::instance().present();

}

GameObject* Scene::addGameObject(std::string gameObjectId, int layer, float xMapPos, float yMapPos, float angle, bool cameraFollow)
{

	/*
	Create a new unique_ptr of game object, emplace in the vector, and then call init on the new gameObject
	We have to call init after construction in order to set the pointer references correctly.i.e all components will store a raw
	pointer to gameObject and all gameObjects will store a raw pointer to the scene.
	*/

	auto& gameObject = m_gameObjects[layer].emplace_back(std::make_shared<GameObject>(gameObjectId, xMapPos, yMapPos, angle));
	gameObject->init(cameraFollow);

	return gameObject.get();




}

//void Scene::addGameObject(GameObject* gameObject, int layer)
//{
//
//	this->m_gameObjects[layer].emplace_back(std::make_shared<GameObject>(*gameObject), this->m_physicsWorld)->init(this);
//
//}

/*
Emplace the new gameObject into the collection and also return a reference ptr to the newly created object as well
*/
GameObject* Scene::addGameObject(std::shared_ptr<GameObject> gameObject, int layer)
{

	auto& gameObjectRef = this->m_gameObjects[layer].emplace_back(gameObject);

	return gameObjectRef.get();

}

void Scene::addKeyAction(SDL_Keycode keyCode, SceneAction sceneAction)
{
	m_sceneKeyActions.emplace(keyCode, sceneAction);
}

void Scene::clearEvents()
{

	//First delete all player input events from last loop
	//m_PlayerInputEvents.clear();
}

void Scene::applyCurrentControlMode()
{

	Game::instance().setInputControlMode(m_inputControlMode);
}

void Scene::setInputControlMode(int inputControlMode)
{

	m_inputControlMode = inputControlMode;
	applyCurrentControlMode();

}

SDL_FPoint Scene::calcWindowPosition(int globalPosition)
{
	SDL_FPoint globalPoint = {};

	if (globalPosition == WindowPosition::CENTER) {

		globalPoint.x = (float)round(GameConfig::instance().windowWidth() / Game::instance().worldTileWidth() / 2);
		globalPoint.y = (float)round(GameConfig::instance().windowHeight() / Game::instance().worldTileHeight() / 2);

	}
	else {
		/* Need other calcs added*/
	}

	return globalPoint;

}

void Scene::_processGameObjectInterdependecies()
{

	for (auto& layer : m_gameObjects) {

		for (auto& gameObject : layer) {

			gameObject->postInit(m_gameObjects);

		}

	}



}