#include "Game.h"

#include "config_data/scenes.h"
#include <string>
#include <gsl/gsl>
#include "Level.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "Renderer.h"
#include "DynamicTextManager.h"
#include "ParticleMachine.h"
#include "GameConfig.h"
#include "Camera.h"
#include "Clock.h"
#include "ObjectPoolManager.h"
#include "DebugPanel.h"
#include "components/ActionComponent.h"
#include "ContactFilter.h"
#include "ContactListener.h"
#include "Globals.h"

#include "config_data/scenes.h"
#include "config_data/GameDefinitions.h"


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

	TTF_Quit();


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

		//Initialize the camera
		Camera::instance().init();

		//Init font library
		TTF_Init();

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

		//Initialize the texture manager
		Renderer::instance().init(m_window);

		//Initialize the texture manager
		TextureManager::instance().init(m_window);

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
		//ObjectPoolManager::instance().init();

		//Initilaize the SceneManager
		SceneManager::instance().init();

		//Initialize the clock object
		Clock::instance().init();

		//Load the First level
		//Level::instance().load("level1");

		//Initilaize Camera size and
		Camera::instance().init();

	}

	//Load a first scene
	Scene& scene = SceneManager::instance().pushScene("SCENE_PLAY");
	scene.applyCurrentControlMode();

	//Load the player and some other objects
	auto playerObject = scene.addGameObject("GINA_64", LAYER_MAIN, 8, 8, 0, true);
	auto weaponObject = scene.addGameObject("WEAPON1", LAYER_MAIN, 8, 8, 0, true);
	playerObject->addInventoryItem(weaponObject);

	scene.addGameObject("FPS_VALUE", LAYER_TEXT, 1, 1);
	scene.addGameObject("SWORDLADY", LAYER_MAIN, 1, 1);

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








