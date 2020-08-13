#include "Scene.h"

#include "Level.h"
#include "Clock.h"
#include "Camera.h"
#include "game.h"
#include "Renderer.h"
#include "Globals.h"
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
		auto locationX = gameObjectJSON["location"]["x"].asInt();
		auto locationY = gameObjectJSON["location"]["y"].asInt();
		auto layer = EnumMap::instance().toEnum(gameObjectJSON["layer"].asString());
		addGameObject(id, layer, locationX, locationY, 0);
	}

	//Keycode actions
	for (Json::Value keyActionJSON : definitionJSON["keyActions"]) {

		auto keyCode = EnumMap::instance().toEnum(keyActionJSON["keyCode"].asString());

		auto action = EnumMap::instance().toEnum(keyActionJSON["sceneAction"]["action"].asString());
		auto newSceneId = keyActionJSON["sceneAction"]["newSceneId"].asString();

		SceneAction sceneAction = { int(action) , newSceneId };
		addKeyAction(keyCode, sceneAction);
	}



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

void Scene::init(size_t mouseMode, std::string levelId, SDL_Keycode exitKey, size_t maxObjects)
{

	for (auto& gameLayer : m_gameObjects)
	{
		gameLayer.reserve(maxObjects);
	}

	//Load the First level
	if (levelId.empty() == false)
	{
		Level::instance().load(levelId, this);
	}

	m_state = SceneState::RUN;

	/*m_physicsWorld = new b2World(GameConfig::instance().gravity());
	m_physicsWorld->SetAutoClearForces(true);*/

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


	// spin through list of particle tasks to execute, like exposions and emitters
	m_particleMachine.update();

	Camera::instance().update();

	//Update all of the other non player related update chores for each game object
	// Game objects are stored in layers
	for (auto& gameObjects : m_gameObjects)
	{
		for (int i = 0; i < gameObjects.size(); i++)
		{

			if (gameObjects[i]->removeFromWorld())
			{
				gameObjects[i]->setRemoveFromWorld(false);
				gameObjects[i]->reset();
				gameObjects.erase(gameObjects.begin() + i);
			}
			else
			{
				gameObjects[i]->update();
			}
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
	for (auto& gameLayer : m_gameObjects)
	{
		//Update normal game objects
		for (auto& gameObject : gameLayer)
		{
			gameObject->render();
		}
	}

	//DebugDraw
	//if (GameConfig::instance().b2DebugDrawMode() == true)
	//{
	//	m_physicsWorld->DrawDebugData();
	//}

	////Push all drawn things to the graphics display
	//Renderer::instance().present();

}

void Scene::addGameObject(std::string gameObjectId, int layer, float xMapPos, float yMapPos, float angle, bool cameraFollow)
{

	/*
	Create a new unique_ptr of gameobject, emplace in the vector, and then call init on the new gameObject
	We have to call init after construction in order to set the pointer refrences correctly.i.e all components will store a raw
	pointer to gameObject and all gameObjects will store a raw pointer to the scene.
	*/
	this->m_gameObjects[layer].emplace_back(std::make_shared<GameObject>(gameObjectId, xMapPos, yMapPos, angle))->init(cameraFollow);

	//std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>(gameObjectId, xMapPos, yMapPos, angle);
	//this->m_gameObjects[layer].emplace_back(std::move(gameObject))->init(this);




}

//void Scene::addGameObject(GameObject* gameObject, int layer)
//{
//
//	this->m_gameObjects[layer].emplace_back(std::make_shared<GameObject>(*gameObject), this->m_physicsWorld)->init(this);
//
//}

void Scene::addGameObject(std::shared_ptr<GameObject> gameObject, int layer)
{

	this->m_gameObjects[layer].push_back(gameObject);

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

