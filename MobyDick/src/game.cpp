#include "Game.h"

#include <string>

#include "Level.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "Renderer.h"
#include "DynamicTextManager.h"
#include "ParticleMachine.h"
#include "GameConfig.h"
#include "Camera.h"
//#include "GUIEvent.h"
#include "Clock.h"
#include "ObjectPoolManager.h"
#include "EventManager.h"
#include "DebugPanel.h"
#include "components/ActionComponent.h"


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

	delete m_player;

	//Delete box2d world - should delete all bodies and fixtures within
	delete m_physicsWorld;

	TTF_Quit();


}

Game::Game()
{

	m_physicsWorld = nullptr;

	m_window = nullptr;

	m_player = nullptr;

}

/*
Initialize Game
*/
bool Game::init()
{

	m_gameState = GameState::PLAY;


	//Get all of the configuration values
	GameConfig::instance().init("gameConfig");
	
	//Reserve each layors vector for efficient object memory management
	for (auto& gameLayer : m_gameObjects)
	{
		gameLayer.reserve(4000);
	}

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
		GameObjectManager::instance().init();

		//Initilaze the Particle Pool Manager
		ObjectPoolManager::instance().init();

		//Set the mouse mode
		SDL_ShowCursor(false);
		SDL_SetRelativeMouseMode(SDL_TRUE);

		//Initialize the clock object
		Clock::instance().init();

		//Load the First level
		Level::instance().load("level1");

		//Initilaize Camera size and
		Camera::instance().init();

		
	}

	//m_gameObjects[GameObjectLayer::MAIN].emplace_back(std::make_shared<GameObject>("SWORDLADY", 0.f, 0.f, 0.f));

	//for (int i = 0; i < 600; i++)
	std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>("GINA_64", 5.f, 5.f, 0.f);
	Game::instance().addGameObject(gameObject, GameObjectLayer::MAIN);
	gameObject->addInventoryItem(std::make_shared<GameObject>("WEAPON1", 5.f, 5.f, 0.f));
//	for (int i = 0; i < 10000; i++)
		m_gameObjects[GameObjectLayer::MAIN].emplace_back(std::make_shared<GameObject>("SWORDLADY", 5.f, 5.f, 0.f));

	//m_gameObjects[GameObjectLayer::BACKGROUND].emplace_back(std::make_shared<GameObject>("PLAYER_LABEL", 7.f, 7.f, 0.f));

	m_gameObjects[GameObjectLayer::DEBUG].emplace_back(std::make_shared<GameObject>("FPS_VALUE", 1.f, 1.f, 0.f));


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
	if (Clock::instance().hasMetGameLoopSpeed())
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



void Game::renderGameObjects(const std::array <std::vector<GameObject>, MAX_GAMEOBJECT_LAYERS>& gameObjects)
{

}


void Game::addGameObject(std::string gameObjectId, int layer, float xMapPos, float yMapPos, float angle)
{

	this->m_gameObjects[layer].emplace_back(std::make_shared<GameObject>(gameObjectId, xMapPos, yMapPos, angle));
	

}

void Game::addGameObject(std::shared_ptr<GameObject>gameObject, int layer)
{

	this->m_gameObjects[layer].emplace_back(gameObject);


}




void Game::_update() {


	//Specifiaclly handle input and stuff for the one player gameObject
	//m_player->update();

	//Update the camera frame to point to the new player position
	//

	// spin through list of particle tasks to execute, like exposions and emitters
	ParticleMachine::instance().update();

	//Update all of the other non player related update chores for each game object
	// Game objects are stored in layers
	for (auto& gameObjects : m_gameObjects)
	{

		for (int i = 0; i < gameObjects.size(); i++)
		{


			//TODO:Instead of this, give the camera an object/position to follow
			//and the camera will follow on its own
			//
			if (gameObjects[i]->id().compare("GINA_64") == 0) {
				Camera::instance().setFramePosition(
					(gameObjects[i]->getComponent<TransformComponent>()->position().x) -
					(Camera::instance().frame().w / 2),
					(gameObjects[i]->getComponent<TransformComponent>()->position().y) -
					(Camera::instance().frame().h / 2));
			}


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
		//if (particleObject->removeFromWorld() == true)
		//{
		//	particleObjectRemoved = particleObject;
		//	gameObject.reset();
		//	std::swap(gameObjectCollection.particleObjects()[x],
		//		gameObjectCollection.particleObjects()[gameObjectCollection.particleObjects().size() - 1]);
		//	gameObjectCollection.particleObjects().resize(gameObjectCollection.particleObjects().size() - 1);
		//}

		//resize the particle vector in case items were removed
		//gameObjectCollection.particleObjects().shrink_to_fit();

	}


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

	//render the player
	//m_player->render();

	//Render all of the game objects in thew world
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





