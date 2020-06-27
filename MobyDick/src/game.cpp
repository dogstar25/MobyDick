#include "Game.h"

#include "GameObject.h"
#include "PlayerObject.h"
#include "TextObject.h"
#include "ParticleObject.h"
#include "CompositeObject.h"
#include "Level.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "DynamicTextManager.h"
#include "ParticleMachine.h"
#include "GameConfig.h"
#include "Camera.h"
#include "WeaponObject.h"
#include "GUIEvent.h"
#include "Clock.h"
#include "ObjectPoolManager.h"


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
	TTF_Quit();

	for (int x = 0; x < constants::MAX_GAMEOBJECT_LAYERS; x++)
	{
		m_gameCollections[x].gameObjects().clear();
		m_gameCollections[x].particleObjects().clear();
	}

	delete m_player;

	//Delete box2d world - should delete all bodies and fixtures within
	delete m_physicsWorld;


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


	
	//Get all of the configuration values
	GameConfig::instance().init("gameConfig");
	

	//Initialize world
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{

		//Initialize the camera
		//Camera::instance().init();

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
		TextureManager::instance().init(m_window);

		//Initialize the sound manager
		SoundManager::instance().initSound();
		SoundManager::instance().playMusic("MUSIC_AMBIENCE_1", -1);

		// Construct a physics world object, which will hold and simulate the physics objects.
		m_physicsWorld = new b2World(GameConfig::instance().gravity());
		m_physicsWorld->SetAutoClearForces(true);
		//Add a collision contact listener
		m_physicsWorld->SetContactListener(&m_gameObjectContactListner);

		//Debug Mode
		if (GameConfig::instance().b2DebugDrawMode() == true)
		{
			DebugDraw::instance().SetFlags(DebugDraw::e_shapeBit);
			m_physicsWorld->SetDebugDraw(&DebugDraw::instance());
		}

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

	//Allocate the array of vectors for all game objects
	//this->gameObjects = vector<unique_ptr<GameObject>>[this->MAX_LAYERS];

	
	GameObject* gameObject = NULL;
	PlayerObject* playerObject = NULL;
	WorldObject* worldObject = NULL;
	TextObject* textObject = NULL;
	CompositeObject* compositeObject = NULL;

	//Create the main player object
	m_player = GameObjectManager::instance().buildGameObject <PlayerObject>("GINA_64", 4, 4, 0);
	m_player->addWeapon("WEAPON1");

	//set camera to center on player object
	//
	//TODO:Can we remove this?
	//
	Camera::instance().setFramePosition(
		(m_player->physicsBody()->GetPosition().x * GameConfig::instance().scaleFactor()) -
		(Camera::instance().frame().w / 2),
		(m_player->physicsBody()->GetPosition().y * GameConfig::instance().scaleFactor()) -
		(Camera::instance().frame().h / 2));

	//CREATE A TEST TEXT ITEM          
	textObject = GameObjectManager::instance().buildGameObject <TextObject>("FPS_LABEL", 0, 0, 0);
	this->addGameObject(textObject, GameOjectLayer::TEXT);

	//CREATE A DYNAMIC TEST TEXT ITEM
	textObject = GameObjectManager::instance().buildGameObject <TextObject>("FPS_VALUE", 0, 1, 0);
	this->addGameObject(textObject, GameOjectLayer::TEXT);
	
	gameObject = GameObjectManager::instance().buildGameObject <GameObject>("SWORDLADY", 1, 1, 0);
	this->addGameObject(gameObject, GameOjectLayer::MAIN);

	//gameObject = gameObjectManager.buildGameObject <GameObject>("ROCK128", 13, 13, 0);
	//this->addGameObject(gameObject, this->MAIN);

	compositeObject = GameObjectManager::instance().buildGameObject <CompositeObject>("DRONE", 11, 11, 0);
	this->addGameObject(compositeObject, GameOjectLayer::MAIN);

//	worldObject = GameObjectManager::instance().buildGameObject <WorldObject>("CHAINTEST", 3, 3, 0);
//	this->addGameObject(worldObject, GameOjectLayer::MAIN);

	//gameObject = GameObjectManager::instance().buildGameObject <GameObject>("PARTICLE_SMOKE_GLOW", 4, 4, 45);
	//this->addGameObject(gameObject, GameOjectLayer::MAIN);

	

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

	_handleEvents();

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



void Game::renderCollection(const std::array<GameObjectCollection, constants::MAX_GAMEOBJECT_LAYERS>& gameObjectCollection)
{

	//Render all of the game objects
	for (auto collection : gameObjectCollection)
	{
		for (auto& gameObject : collection.gameObjects())
		{
			gameObject->render();
		}

		for (auto& particleObject : collection.particleObjects())
		{
			particleObject->render();
		}
	}
}


void Game::addGameObject(GameObject* gameObject, int layer)
{

	//this->gameObjects[layer].push_back(make_unique<GameObject>(*gameObject));
	m_gameCollections[layer].gameObjects().push_back(gameObject);

}

void Game::addGameObject(WorldObject* gameObject, int layer)
{

	//this->gameObjects.push_back(unique_ptr<WorldObject>(gameObject));
	m_gameCollections[layer].gameObjects().push_back(gameObject);

}

void Game::addGameObject(ParticleObject* gameObject, int layer)
{
	//this->gameObjects.push_back(unique_ptr<WorldObject>(gameObject));
	gameObject->time_snapshot = std::chrono::steady_clock::now();
	m_gameCollections[layer].particleObjects().push_back(gameObject);
}

void Game::addGameObject(TextObject* gameObject, int layer)
{
	//this->gameObjects.push_back(unique_ptr<WorldObject>(gameObject));
	m_gameCollections[layer].gameObjects().push_back(gameObject);
}

void Game::addGameObject(CompositeObject* gameObject, int layer)
{

	m_gameCollections[layer].gameObjects().push_back(gameObject);
}

void Game::addGameObject(WeaponObject* gameObject, int layer)
{

	m_gameCollections[layer].gameObjects().push_back(gameObject);
}


void Game::_update() {


	//Specifiaclly handle input and stuff for the one player gameObject
	m_player->update();

	//Update the camera frame to point to the new player position
	//
	//TODO:Instead of this, give the camera an object/position to follow
	//and the camera will follow on its own
	//
	Camera::instance().setFramePosition(
		(m_player->physicsBody()->GetPosition().x * GameConfig::instance().scaleFactor()) -
		(Camera::instance().frame().w / 2),
		(m_player->physicsBody()->GetPosition().y * GameConfig::instance().scaleFactor()) -
		(Camera::instance().frame().h / 2));

	// spin through list of particle tasks to execute, like exposions and emitters
	ParticleMachine::instance().update();

	//Update all of the other non player related update chores for each game object
	// Game objects are stored in layers
	for (auto& gameObjectCollection : m_gameCollections)
	{
		//Update normal game objects
		for (auto& gameObject : gameObjectCollection.gameObjects())
		{
			gameObject->update();
		}

		//Update particle game objects
		ParticleObject* particleObject = NULL;
		ParticleObject* particleObjectRemoved = NULL;

		for (int x = 0; x < gameObjectCollection.particleObjects().size(); x++)
		{

			//If particle is expired, reset it and remove from teh game world list
			//The pointer and objectitself will remain in the pool

			particleObject = gameObjectCollection.particleObjects()[x];

			if (particleObject->removeFromWorld() == true)
			{
				particleObjectRemoved = particleObject;
				ObjectPoolManager::instance().reset(particleObject);
				std::swap(gameObjectCollection.particleObjects()[x],
					gameObjectCollection.particleObjects()[gameObjectCollection.particleObjects().size() - 1]);
				gameObjectCollection.particleObjects().resize(gameObjectCollection.particleObjects().size() - 1);
			}
			else
			{
				particleObject->update();
			}

		}

		//resize the particle vector in case items were removed
		gameObjectCollection.particleObjects().shrink_to_fit();

	}

	//Step the box2d physics world
	m_physicsWorld->Step(GameConfig::instance().timeStep(),
		GameConfig::instance().velocityIterations(),
		GameConfig::instance().positionIterations());

}



void Game::_render() {

	//Clear the graphics display
	TextureManager::instance().clear();

	//render the player
	m_player->render();

	//Render all of the game objects in thew world
	renderCollection(m_gameCollections);

	//DebugDraw
	if (GameConfig::instance().b2DebugDrawMode() == true)
	{
		m_physicsWorld->DrawDebugData();
	}

	//Push all drawn things to the graphics display
	TextureManager::instance().present();

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
				std::unique_ptr<GUIEvent> guiEvent = std::make_unique<GUIEvent>("GUIPausePanel");
				guiEvent->run();
			}
			else
			{
				m_player->handlePlayerMovementEvent(&event);
			}
			break;

		case SDL_MOUSEMOTION:
			m_player->handlePlayerMovementEvent(&event);
			break;
		case SDL_MOUSEBUTTONDOWN:
			//this->testSound();
			m_player->fire();
			break;
		case SDL_USEREVENT:
			delete event.user.data1;
			break;
		default:
			break;
		}

		
	}
}





