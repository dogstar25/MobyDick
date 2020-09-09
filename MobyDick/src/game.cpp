#include "Game.h"

#include "GameObjectManager.h"
#include "SoundManager.h"
#include "Camera.h"
#include "Clock.h"
#include "ContactFilter.h"
#include "ContactListener.h"
#include "config_data/GameDefinitions.h"
#include "Renderer_GL.h"
#include "Renderer_SF.h"
#include <SFML/Window.hpp>


using namespace std::chrono_literals;

Game& Game::instance()
{
	static Game singletonInstance;
	return singletonInstance;
}


Game::~Game()
{

	printf("cleaning game\n");

	//Delete SDL stuff
	SDL_DestroyWindow(m_window);
	SDL_Quit();
	

	for (int x = 0; x < MAX_GAMEOBJECT_LAYERS; x++)
	{
		m_gameObjects[x].clear();
	}

	////Delete box2d world - should delete all bodies and fixtures within
	delete m_physicsWorld;



}

Game::Game()
{

	//m_physicsWorld = nullptr;

	m_window = nullptr;

}

/*
Initialize Game
*/
bool Game::init()
{

	//string it = todd::testit[0].id;
	//SceneConfigData::scenes;

	//std::string test = todd::testit[0].id;

	m_gameState = GameState::PLAY;

	//Get all of the configuration values
	GameConfig::instance().init("gameConfig");
	
	//Initialize world
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{

		//Initialize some pre-windowd creation openGl stuff
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		//Create the game window
		uint16 windowFlags = 0 | SDL_WINDOW_OPENGL;
		if (GameConfig::instance().windowFullscreen() == true) 
		{
			windowFlags = windowFlags | SDL_WINDOW_FULLSCREEN;

		}
		else
		{
			windowFlags = windowFlags | SDL_WINDOW_RESIZABLE;
		}

		m_window = SDL_CreateWindow(GameConfig::instance().gameTitle().c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			GameConfig::instance().windowWidth(),
			GameConfig::instance().windowHeight(),
			windowFlags);




		//Initialize the renderer
		Renderer::instance().init(m_window);
		Renderer_SF::instance().init();

		//Renderer_GL::instance().init(m_window);
		//Renderer_GL::instance().openGLSetup(m_window);

		//Initialize the texture manager
		TextureManager::instance().init();

		//Initialize the sound manager
		SoundManager::instance().initSound();
		//SoundManager::instance().playMusic("MUSIC_AMBIENCE_1", -1);

		// Construct a physics world object, which will hold and simulate the physics objects.
		m_physicsWorld = new b2World(GameConfig::instance().gravity());
		m_physicsWorld->SetAutoClearForces(true);

		//Add a collision contact listener and filter for box2d callbacks
		m_physicsWorld->SetContactListener(&ContactListener::instance());
		m_physicsWorld->SetContactFilter(&ContactFilter::instance());

		//Debug Mode
		/*if (GameConfig::instance().b2DebugDrawMode() == true)
		{
			DebugDraw::instance().SetFlags(DebugDraw::e_shapeBit);
			m_physicsWorld->SetDebugDraw(&DebugDraw::instance());
		}*/

		//Initilaze the Game Object Manager
		GameObjectManager::instance().init();

		//Initilaze the Particle Pool Manager
		ObjectPoolManager::instance().init();

		//Initilaize the SceneManager
		SceneManager::instance().init();

		//Initialize the clock object
		Clock::instance().init();

		//Load the First level
		//Level::instance().load("level1");
	}

	//Load a first scene
	Scene& scene = SceneManager::instance().pushScene("SCENE_PLAY");
	scene.applyCurrentControlMode();
	scene.addGameObject("BOWMAN", LAYER_MAIN, 2, 2, 0);

	//Load the player and some other objects
	/*auto playerObject = scene.addGameObject("GINA_64", LAYER_MAIN, 8, 8, 0, true);
	auto weaponObject = scene.addGameObject("PISTOL", LAYER_MAIN, 8, 8, 0, true);
	playerObject->addInventoryItem(weaponObject);

	
	scene.addGameObject("FPS_VALUE", LAYER_TEXT, 1, 1);*/
	//scene.addGameObject("SWORDLADY", LAYER_MAIN, 10, 1);

	auto test = GameDefs::instance().gina_64;


	return true;
}


/*
Main Play Loop
*/
void Game::play()
{


	while (m_gameState != GameState::QUIT)
	{

		std::optional<SceneAction> action = SceneManager::instance().pollEvents();

		if (action.has_value()) {
			if (action->actionCode == SCENE_ACTION_QUIT) {
				m_gameState = GameState::QUIT;
			}
			else if (action->actionCode == SCENE_ACTION_EXIT) {
				SceneManager::instance().popScene();
			}
			else if (action->actionCode == SCENE_ACTION_ADD) {
				SceneManager::instance().pushScene(action->sceneId);
			}
			else if (action->actionCode == SCENE_ACTION_REPLACE) {
				SceneManager::instance().popScene();
				SceneManager::instance().pushScene(action->sceneId);
			}

			//Apply the mouse control mode based on what the new "top" scene wants
			SceneManager::instance().scenes().back().applyCurrentControlMode();
		}

		SceneManager::instance().run();
	}

}

void Game::setInputControlMode(int inputControlMode)
{
	if (inputControlMode == CONTROL_MODE_PLAY) {

		SDL_ShowCursor(false);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else if(inputControlMode == CONTROL_MODE_SELECT) {
		SDL_ShowCursor(true);
		SDL_SetRelativeMouseMode(SDL_FALSE);

	}
}

void Game::addGameObject(std::shared_ptr<GameObject>gameObject, int layer)
{
	//Add the gameObject to the currently active scene using back()
	SceneManager::instance().scenes().back().addGameObject(gameObject, layer);


}

GameObject* Game::addGameObject(std::string gameObjectId, int layer, float xMapPos, float yMapPos, float angle, bool cameraFollow)
{
	//Add the gameObject to the currently active scene using back()
	auto gameObject = SceneManager::instance().scenes().back().addGameObject(gameObjectId, layer, xMapPos, yMapPos, angle, cameraFollow);

	return gameObject;

}





