#include "Game.h"

#include <string>

#include "Level.h"
#include "TextureManager.h"
#include "EntityDefinitionManager.h"
#include "SoundManager.h"
#include "Renderer.h"
#include "DynamicTextManager.h"
//#include "ParticleMachine.h"
#include "GameConfig.h"
#include "Camera.h"
//#include "GUIEvent.h"
#include "Clock.h"
//#include "ObjectPoolManager.h"
#include "EventManager.h"
#include "DebugPanel.h"
#include "components/ActionComponent.h"
#include "systems/RenderSystem.h"


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

	//Delete box2d world - should delete all bodies and fixtures within
	delete m_physicsWorld;

	TTF_Quit();

}

Game::Game()
{

	m_physicsWorld = nullptr;

	m_window = nullptr;

}




/*
Initialize Game
*/
bool Game::init()
{

	m_gameState = GameState::PLAY;

	m_gameCoordinator.init();

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
		SoundManager::instance().playMusic("MUSIC_AMBIENCE_1", -1);

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
		EntityDefinitionManager::instance().init();

		//Initilaze the Particle Pool Manager
		//ObjectPoolManager::instance().init();

		//Set the mouse mode
		SDL_ShowCursor(false);
		SDL_SetRelativeMouseMode(SDL_TRUE);

		//Initialize the clock object
		Clock::instance().init();

		//Initialize various entity systems
		m_renderSystem = m_gameCoordinator.GetSystem<RenderSystem>();
		m_renderSystem->init();
		m_animationSystem = m_gameCoordinator.GetSystem<AnimationSystem>();
		m_animationSystem->init();
		m_physicsSystem = m_gameCoordinator.GetSystem<PhysicsSystem>();
		m_physicsSystem->init();
		m_playerControlSystem = m_gameCoordinator.GetSystem<PlayerControlSystem>();
		m_playerControlSystem->init();

		//Load the First level
		Level::instance().load("level1");

		
	}


	//m_gameCoordinator.addEntity("BOWMAN", 4, 4, 0);
	//m_gameCoordinator.addEntity("SWORDLADY", 6, 6, 0);
	m_gameCoordinator.addEntity("GINA_64", 6, 6, 0);
	m_gameCoordinator.addEntity("GINA_64", 7, 6, 0);
	m_gameCoordinator.addEntity("GINA_64", 8, 6, 0);
	m_gameCoordinator.addEntity("GINA_64", 9, 6, 0);
	m_gameCoordinator.addEntity("GINA_64", 6, 8, 0);


	return true;
}


/*
Main Play Loop
*/
void Game::play()
{


	//Capture the amount of time that has passed since last loop and accumulate time for both
	//the FPS calculation and the game loop timer
	Clock::instance().update();

	//_handleEvents();
	EventManager::instance().pollEvents();

	//Only update and render if we have passed the 60 fps time passage
	//if (Clock::instance().hasMetGameLoopSpeed())
	{
		//Handle updating objects positions and physics
		_update();

		//render everything
		_render();

		//Increment frame counter and calculate FPS and reset the gameloop timer
		Clock::instance().calcFps();

		DynamicTextManager::instance().updateText("FPS_VALUE", std::to_string(Clock::instance().fps()));

		

	}

}



void Game::_update() {

	m_playerControlSystem->update();
	m_physicsSystem->update();
	m_animationSystem->update();
	m_renderSystem->update();
	

	/*DebugPanel::instance().addItem("Test", util::generateRandomNumber(1,10000), 8);
	DebugPanel::instance().addItem("Test2", util::generateRandomNumber(1, 10000), 8);
	DebugPanel::instance().addItem("Test3", util::generateRandomNumber(1, 10000), 8);
	DebugPanel::instance().addItem("Test4", util::generateRandomNumber(1, 10000), 8);
	DebugPanel::instance().addItem("Test5", util::generateRandomNumber(1, 10000), 8);
	DebugPanel::instance().addItem("Test6", util::generateRandomNumber(1, 10000), 8);
	DebugPanel::instance().addItem("Test7", util::generateRandomNumber(1, 10000), 8);
	DebugPanel::instance().addItem("Test8", util::generateRandomNumber(1, 10000), 8);
	DebugPanel::instance().addItem("Test9", util::generateRandomNumber(1, 10000), 8);*/



	//Clear all events
	EventManager::instance().clearEvents();

	//Step the box2d physics world
	m_physicsWorld->Step(GameConfig::instance().timeStep(),
		GameConfig::instance().velocityIterations(),
		GameConfig::instance().positionIterations());

}



void Game::_render() {

	//Clear the graphics display
	Renderer::instance().clear();

	m_renderSystem->update();

	//DebugDraw
	//if (GameConfig::instance().b2DebugDrawMode() == true)
	//{
	//	m_physicsWorld->DrawDebugData();
	//}

	//Push all drawn things to the graphics display
	Renderer::instance().present();

}

void Game::_handleEvents() {
	SDL_Event event;

	if (SDL_PollEvent(&event)) {

		switch (event.type) 
		{
		case SDL_QUIT:
			m_gameState = QUIT;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			if ((char)event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN)
			{
				//std::unique_ptr<GUIEvent> guiEvent = std::make_unique<GUIEvent>("GUIPausePanel");
				m_gameState = QUIT;
				//guiEvent->run();
			}
			break;
		case SDL_USEREVENT:
			delete event.user.data1;
			break;
		default:
			break;
		}

		
	}
}





