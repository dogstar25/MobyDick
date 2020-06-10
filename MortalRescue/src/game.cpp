#include "Game.h"
#include "PlayerObject.h"
#include "TextObject.h"
#include "WorldObject.h"
#include "ParticleObject.h"
#include "CompositeObject.h"
#include "Level.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "DynamicTextManager.h"
#include "GameObjectCollection.h"
#include "ParticleMachine.h"
#include "GameObject.h"
#include "GameConfig.h"
#include "Util.h"
#include "Camera.h"
#include "WeaponObject.h"
#include "ParticleEmission.h"
#include "GUIEvent.h"
#include "Clock.h"


using namespace chrono_literals;
using namespace std;

Game::~Game()
{

	printf("cleaning game\n");

	//Delete SDL stuff
	SDL_DestroyWindow(this->pWindow);
	SDL_Quit();
	TTF_Quit();

	for (int x = 0; x < constants::MAX_GAMEOBJECT_LAYERS; x++)
	{
		this->gameCollections[x].gameObjects.clear();
		this->gameCollections[x].particleObjects.clear();
	}

	delete this->player;

	//Delete box2d world - should delete all bodies and fixtures within
	delete this->physicsWorld;


}

Game::Game()
{

	this->worldBounds = {};
	this->worldGridSize = {};
	this->physicsWorld = nullptr;

	this->pWindow = nullptr;
	this->gameObjectCount = 0;
	this->mouseSensitivity = 0;
	this->mouseLocation = {};
	this->mouseClickLocation = {};

	this->player = nullptr;

	this->gameState= GameState::PLAY;

	this->fps = 0;


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
		Camera::instance().init();

		//Init font library
		TTF_Init();

		//Init Game State
		this->gameState = GameState::PLAY;

		//Create the game window
		pWindow = SDL_CreateWindow(GameConfig::instance().gameTitle().c_str(),
			this->windowXpos,
			this->windowYPos,
			Camera::instance().frame().w,
			Camera::instance().frame().h,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

		//Initialize the texture manager
		TextureManager::instance().init(pWindow);

		//Initialize the sound manager
		SoundManager::instance().initSound();
		SoundManager::instance().playMusic("MUSIC_AMBIENCE_1", -1);

		// Construct a physics world object, which will hold and simulate the physics objects.
		this->physicsWorld = new b2World(GameConfig::instance().gravity());
		this->physicsWorld->SetAutoClearForces(true);
		//Add a collision contact listener
		this->physicsWorld->SetContactListener(&m_gameObjectContactListner);

		//Debug Mode
		if (GameConfig::instance().b2DebugDrawMode() == true)
		{
			this->debugDraw.SetFlags(DebugDraw::e_shapeBit);
			this->physicsWorld->SetDebugDraw(&this->debugDraw);
		}

		//Initilaze the Game Object Manager
		GameObjectManager::instance().init();

		//Initilaze the Particle Pool Manager
		ObjectPoolManager::instance().init();

		//Set the mouse mode
		SDL_ShowCursor(false);
		SDL_SetRelativeMouseMode(SDL_TRUE);

		//Build the world for a particular level
		this->buildWorld("TX_LEVEL1_BLUEPRINT");

		//Initialize the clock object
		Clock::instance().init();
		
	}

	//Allocate the array of vectors for all game objects
	//this->gameObjects = vector<unique_ptr<GameObject>>[this->MAX_LAYERS];

	
	GameObject* gameObject = NULL;
	PlayerObject* playerObject = NULL;
	WorldObject* worldObject = NULL;
	TextObject* textObject = NULL;
	CompositeObject* compositeObject = NULL;

	//Create the main player object
	playerObject = GameObjectManager::instance().buildGameObject <PlayerObject>("GINA_64", 4, 4, 0);
	playerObject->addWeapon("WEAPON1");
	this->player = playerObject;


	//set camera to center on player object
	//
	//TODO:Can we remove this?
	//
	Camera::instance().setFramePosition(
		(this->player->physicsBody()->GetPosition().x * GameConfig::instance().scaleFactor()) -
		(Camera::instance().frame().w / 2),
		(this->player->physicsBody()->GetPosition().y * GameConfig::instance().scaleFactor()) -
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

	//Create the debug panel if its turned on
	if (GameConfig::instance().debugPanel() == true)
	{

		this->debugPanel = make_unique<DebugPanel>();

	}




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

	//Only update and render if we have passed the 60 fps time passage
	if (Clock::instance().hasMetGameLoopSpeed())
	{
		//Handle updating objects positions and physics
		update();

		//render everything
		render();

		//Increment frame counter and calculate FPS and reset the gameloop timer
		Clock::instance().calcFps();

		DynamicTextManager::instance().updateText("FPS_VALUE", to_string(Clock::instance().fps()));

	}

}



void Game::update() {


	//Specifiaclly handle input and stuff for the one player gameObject
	this->player->update();

	//Update the camera frame to point to the new player position
	//
	//TODO:Instead of this, give the camera an object/position to follow
	//and the camera will follow on its own
	//
	Camera::instance().setFramePosition(
		(this->player->physicsBody()->GetPosition().x * GameConfig::instance().scaleFactor()) -
		(Camera::instance().frame().w / 2),
		(this->player->physicsBody()->GetPosition().y * GameConfig::instance().scaleFactor()) -
		(Camera::instance().frame().h / 2));

	// spin through list of particle tasks to execute, like exposions and emitters
	ParticleMachine::instance().update();

	//Update all of the other non player related update chores for each game object
	// Game objects are stored in layers
	for (auto & gameObjectCollection : this->gameCollections)
	{
		//Update normal game objects
		for (auto & gameObject : gameObjectCollection.gameObjects)
		{
			gameObject->update();
		}

		//Update particle game objects
		ParticleObject* particleObject = NULL;
		ParticleObject* particleObjectRemoved = NULL;

		for (int x=0;x<gameObjectCollection.particleObjects.size();x++)
		{

			//If particle is expired, reset it and remove from teh game world list
			//The pointer and objectitself will remain in the pool
			
			particleObject = gameObjectCollection.particleObjects[x];

			if (particleObject->removeFromWorld() == true)
			{
				particleObjectRemoved = particleObject;
				ObjectPoolManager::instance().reset(particleObject);
				std:swap(gameObjectCollection.particleObjects[x], 
					gameObjectCollection.particleObjects[gameObjectCollection.particleObjects.size()-1]);
				gameObjectCollection.particleObjects.resize(gameObjectCollection.particleObjects.size() - 1);
			}
			else
			{
				particleObject->update();
			}

		}
		
		//resize the particle vector in case items were removed
		gameObjectCollection.particleObjects.shrink_to_fit();

	}

	//Step the box2d physics world
	this->physicsWorld->Step(GameConfig::instance().timeStep(), 
		GameConfig::instance().velocityIterations(),
		GameConfig::instance().positionIterations());

}



void Game::render() {

	//Clear teh graphics display
	TextureManager::instance().clear();

	//render the player
	this->player->render();

	//Render all of the game objects in thew world
	renderCollection(&this->gameCollections);

	//DebugDraw
	if (GameConfig::instance().b2DebugDrawMode() == true)
	{
		this->physicsWorld->DrawDebugData();
	}

	/*
	SDL_Color color = { 0,0,255,255 };
	b2Vec2 start = { 64,64 };
	b2Vec2 end = { 67,1000 };
	TextureManager::instance().drawGlowLine(start, end, color);
	*/

	//Push all drawn things to the graphics display
	TextureManager::instance().present();

}

void Game::renderCollection(array<GameObjectCollection, constants::MAX_GAMEOBJECT_LAYERS>* gameObjectCollection)
{

	//Render all of the game objects
	for (const auto& collection : *gameObjectCollection)
	{
		for (const auto& gameObject : collection.gameObjects)
		{
			gameObject->render();
		}

		for (const auto& particleObject : collection.particleObjects)
		{
			particleObject->render();
		}
	}
}


void Game::addGameObject(GameObject* gameObject, int layer)
{

	//this->gameObjects[layer].push_back(make_unique<GameObject>(*gameObject));
	this->gameCollections[layer].gameObjects.push_back(gameObject);
	this->gameObjectCount++;

}

void Game::addGameObject(WorldObject* gameObject, int layer)
{

	//this->gameObjects.push_back(unique_ptr<WorldObject>(gameObject));
	this->gameCollections[layer].gameObjects.push_back(gameObject);
	this->gameObjectCount++;
}

void Game::addGameObject(ParticleObject* gameObject, int layer)
{
	//this->gameObjects.push_back(unique_ptr<WorldObject>(gameObject));
	gameObject->time_snapshot = steady_clock::now();
	this->gameCollections[layer].particleObjects.push_back(gameObject);
	this->gameObjectCount++;
}

void Game::addGameObject(TextObject* gameObject, int layer)
{
	//this->gameObjects.push_back(unique_ptr<WorldObject>(gameObject));
	this->gameCollections[layer].gameObjects.push_back(gameObject);
	this->gameObjectCount++;
}

void Game::addGameObject(CompositeObject* gameObject, int layer)
{

	this->gameCollections[layer].gameObjects.push_back(gameObject);
	this->gameObjectCount++;
}

void Game::addGameObject(WeaponObject* gameObject, int layer)
{

	this->gameCollections[layer].gameObjects.push_back(gameObject);
	this->gameObjectCount++;
}


void Game::handleEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		switch (event.type) 
		{
		case SDL_QUIT:
			this->gameState = QUIT;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		case SDL_MOUSEMOTION:
			if ((char)event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN)
			{
				unique_ptr<GUIEvent> guiEvent = make_unique<GUIEvent>("GUIPausePanel");
				guiEvent->run();
			}
			else
			{
				this->mouseLocation.Set(event.motion.x, event.motion.y);
				this->player->handlePlayerMovementEvent(&event);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			//this->testSound();
			this->player->fire();
			break;
		case SDL_USEREVENT:
			delete event.user.data1;
			break;
		default:
			break;
		}

		
	}
}

void Game::buildWorld(string levelId)
{
	//load all of the information needed to build the level
	m_level.load("TX_LEVEL1_BLUEPRINT");

	//Initialize world bounds and gridsize based on current level loaded info
	this->initWorldBounds();

	//Set Camera Bounds
	Camera::instance().setCameraBounds(this->worldBounds);

	//Build the actual level gameobjects
	m_level.build("TX_LEVEL1_BLUEPRINT");

}




void Game::initWorldBounds()
{
	int width, height;

	//If there is no level loaded then default the world size to be the same as the camera size
	if (this->m_level.m_id.empty() == true)
	{
		width = Camera::instance().frame().w;
		height = Camera::instance().frame().h;
	}
	else
	{
		width = m_level.m_width * m_level.m_tileWidth;

		height = m_level.m_height * m_level.m_tileHeight;
	}

	this->worldBounds.x = 0;
	this->worldBounds.y = 0;
	this->worldBounds.w = width;
	this->worldBounds.h = height;
	this->worldGridSize.w = m_level.m_tileWidth;
	this->worldGridSize.h = m_level.m_tileHeight;

}


