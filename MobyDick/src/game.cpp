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
//#include "GUIEvent.h"
#include "Clock.h"
#include "ObjectPoolManager.h"
#include "DebugPanel.h"
#include "components/ActionComponent.h"
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

	string it = todd::testit[0].id;
	//SceneConfigData::scenes;

	//std::string test = todd::commonGameObjects[0].id;


	m_gameState = GameState::PLAY;

	//Get all of the configuration values
	GameConfig::instance().init("gameConfig");
	
	//for (auto& gameLayer : m_gameObjects)
	//{
	//	gameLayer.reserve(4000);
	//}

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
		//Add a collision contact listener
		//m_physicsWorld->SetContactListener(&m_gameObjectContactListner);

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

		//Set the mouse mode
		/*SDL_ShowCursor(false);
		SDL_SetRelativeMouseMode(SDL_TRUE);*/

		//Initialize the clock object
		Clock::instance().init();

		//Load the First level
		//Level::instance().load("level1");

		//Initilaize Camera size and
		Camera::instance().init();

		
	}

	/*Scene* mainScene = SceneManager::instance().addScene();
	mainScene->init(MOUSE_MODE_CONTROLLER, "", 2000);
	mainScene->addGameObject("BOWMAN", GameObjectLayer::MAIN, 4,4,0);
	mainScene->setState(SceneState::RUN);

	std::unique_ptr<Scene> mainScene2 = std::make_unique<Scene>();
	mainScene2->init(MOUSE_MODE_CONTROLLER, "", 2000);
	mainScene2->addGameObject("BOWMAN", GameObjectLayer::MAIN, 4, 4, 0);
	mainScene2->setState(SceneState::RUN);
	SceneManager::instance().addScene(std::move(mainScene2));*/


	//SceneManager::instance().scenes().emplace(std::make_unique<Scene>(GameScene::INTRO));
	//SceneManager::instance().scenes().top()->addGameObject("BOWMAN", GameObjectLayer::MAIN, 4, 4, 0);
	//SceneManager::instance().scenes().top()->setState(SceneState::RUN);
	//SceneManager::instance().scenes().top()->init(MOUSE_MODE_CONTROLLER, "", 2000);

	Scene& temp = SceneManager::instance().pushScene("SCENE_PLAY");
	temp.addGameObject("GINA_64", LAYER_MAIN, 8, 8, 0);
	//temp.init(MOUSE_MODE_CONTROLLER, "level1", SDLK_UNKNOWN, 1000);
	//temp.addGameObject("BOWMAN", GameObjectLayer::MAIN, 4, 4, 0);
	//temp.addGameObject("SWORDLADY", GameObjectLayer::MAIN, 8, 8, 0);
	////temp.addGameObject("GINA_64", GameObjectLayer::MAIN, 8, 8, 0);

	//SceneAction sceneAction = { SCENE_ACTION_ADD , "GAMESCENE_PAUSE_MENU" };
	//temp.addKeyAction(SDLK_ESCAPE, sceneAction);




	/*for (auto& sc : SceneManager::instance().scenes()) {



	}*/



	//m_currentGameSpace->init(MOUSE_MODE_CONTROLLER, "level1", 4000);

	//std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>("GINA_64", 5.f, 5.f, 0.f);
	//m_currentGameSpace->addGameObject("GINA_64", GameObjectLayer::MAIN, 4.f, 4.f, 0.f);

	//std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>("GINA_64", 5.f, 5.f, 0.f);
	//Game::instance().addGameObject(gameObject, GameObjectLayer::MAIN);
	//gameObject->addInventoryItem(std::make_shared<GameObject>("WEAPON1", 5.f, 5.f, 0.f));
	//	m_gameObjects[GameObjectLayer::MAIN].emplace_back(std::make_shared<GameObject>("SWORDLADY", 5.f, 5.f, 0.f));

	////m_gameObjects[GameObjectLayer::BACKGROUND].emplace_back(std::make_shared<GameObject>("PLAYER_LABEL", 7.f, 7.f, 0.f));

	//m_gameObjects[GameObjectLayer::DEBUG].emplace_back(std::make_shared<GameObject>("FPS_VALUE", 1.f, 1.f, 0.f));


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
		}

		SceneManager::instance().run();
	}

}










