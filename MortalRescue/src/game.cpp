#include "Game.h"
#include "PlayerObject.h"
#include "TextObject.h"
#include "WorldObject.h"
#include "ParticleObject.h"
#include "CompositeObject.h"
#include "LevelManager.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "DynamicTextManager.h"
#include "GameObjectCollection.h"
#include "ParticleMachine.h"
#include "GameObject.h"
#include "Util.h"
#include "Camera.h"
#include "WeaponObject.h"
#include "ParticleEmission.h"
#include "GUIEvent.h"


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

	this->config = {};
	this->camera = {};
	this->worldBounds = {};
	this->worldGridSize = {};
	this->physicsWorld = nullptr;

	this->pWindow = nullptr;
	this->gameObjectCount = 0;
	this->mouseSensitivity = 0;
	this->mouseLocation = {};
	this->mouseClickLocation = {};

	this->player = nullptr;

	this->gravity = {};
	this->b2DebugDrawMode = false;
	this->timeStep = 0;
	this->velocityIterations = 0;
	this->positionIterations = 0;

	this->gameState= GameState::PLAY;

	this->fps = 0;
	this->awakeCount = 0;
	this->gameLoopStep = 0;



}
/*
Initialize Game
*/
bool Game::init()
{




	//Get all of the configuration values
	getConfig();
	gameObjectCount = 0;

	//Initialize world
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{

		//Init font library
		TTF_Init();

		//Init Game State
		this->gameState = GameState::PLAY;

		//Create the game window
		pWindow = SDL_CreateWindow(this->gameTitle.c_str(),
			this->windowXpos,
			this->windowYPos,
			this->camera.frame.w,
			this->camera.frame.h,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

		//Initialize the texture manager
		TextureManager::instance().init(pWindow);

		//Initialize the sound manager
		SoundManager::instance().initSound();
		SoundManager::instance().playMusic("MUSIC_AMBIENCE_1", -1);

		// Construct a physics world object, which will hold and simulate the physics objects.
		this->physicsWorld = new b2World(this->gravity);
		this->physicsWorld->SetAutoClearForces(true);
		//Add a collision contact listener
		this->physicsWorld->SetContactListener(&m_gameObjectContactListner);

		//Debug Mode
		if (this->b2DebugDrawMode == true)
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
		this->clock.init();
		
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
	this->camera.setPosition((this->player->physicsBody()->GetPosition().x *  this->config.scaleFactor) -
		(camera.frame.w / 2),
		(this->player->physicsBody()->GetPosition().y *  this->config.scaleFactor) -
		(camera.frame.h / 2));

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

	//Create the debug panel if its turned on
	if (this->config.debugPanel == true)
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

	//Get the value for how often to update and render the game
	std::chrono::duration<double> milisecsPerUpdate = std::chrono::milliseconds(this->gameLoopStep);

	//Capture the amount of time that has passed since last loop and accumulate time for both
	//the FPS calculation and the game loop timer
	this->clock.tick();

	//Only update and render if we have passed the 60 fps time passage
	if (this->clock.gameloop_time_accum >= milisecsPerUpdate)
	{
		//Handle updating objects positions and physics
		update();

		//render everything
		render();

		//Increment frame counter and calculate FPS and reset the gameloop timer
		this->clock.current_frame_cnt++;
		this->clock.calcFps();
		this->clock.resetGameLoopTimeAccum();

		DynamicTextManager::instance().updateText("FPS_VALUE", to_string(this->clock.fps));

	}

}



void Game::update() {


	//Specifiaclly handle input and stuff for the one player gameObject
	this->player->update();

	//Update the camera frame to point to the new player position
	this->camera.setPosition((this->player->physicsBody()->GetPosition().x *  this->config.scaleFactor) -
		(camera.frame.w / 2),
		(this->player->physicsBody()->GetPosition().y *  this->config.scaleFactor) -
		(camera.frame.h / 2));

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
	this->physicsWorld->Step(this->timeStep, this->velocityIterations, this->positionIterations);

}



void Game::render() {

	//Clear teh graphics display
	TextureManager::instance().clear();

	//render the player
	this->player->render();
	
	//Render all of the game objects in thew world
	renderCollection(&this->gameCollections);

	//DebugDraw
	if (this->b2DebugDrawMode == true)
	{
		this->physicsWorld->DrawDebugData();
	}

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

bool Game::getConfig()
{
	//Read file and stream it to a JSON object
	Json::Value root;
	ifstream ifs("assets/gameConfig.json");
	ifs >> root;
	//ifs.close();

	//Get and store config values
	this->gameTitle = root["gameTitle"].asString();
	this->gameLoopStep = root["gameLoopStep"].asInt();
	this->gravity.Set(root["physics"]["gravity"]["x"].asInt(), root["physics"]["gravity"]["y"].asFloat());

	this->timeStep = root["physics"]["timeStep"].asFloat();
	this->velocityIterations = root["physics"]["velocityIterations"].asInt();
	this->positionIterations = root["physics"]["positionIterations"].asInt();
	this->b2DebugDrawMode = root["physics"]["b2DebugDrawMode"].asBool();

	this->config.scaleFactor = root["physics"]["box2dScale"].asFloat();
	this->config.mouseSensitivity = root["mouseSensitivity"].asFloat();
	this->config.debugPanel = root["debugPanel"]["show"].asBool();
	this->config.debugPanelLocation.x = root["debugPanel"]["xPos"].asInt();
	this->config.debugPanelLocation.y = root["debugPanel"]["yPos"].asInt();
	this->config.debugPanelFontSize = root["debugPanel"]["fontSize"].asInt();
	this->config.soundChannels = root["sound"]["numberOfChannels"].asInt();

	this->camera.frame.w = root["camera"]["width"].asInt();
	this->camera.frame.h = root["camera"]["height"].asInt();

	

	return true;
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
	m_levelManager.loadLevelBlueprint("TX_LEVEL1_BLUEPRINT");

	//Initialize world bounds and gridsize based on current level loaded info
	this->initWorldBounds();

	//Init Camera
	this->camera.init(&this->worldBounds);

	//Build the actual level gameobjects
	m_levelManager.buildLevel("TX_LEVEL1_BLUEPRINT");

}




void Game::initWorldBounds()
{
	int width, height;

	//If there is no level loaded then default the world size to be the same as the camera size
	if (this->currentLevel.empty())
	{
		width = this->camera.frame.w;
		height = this->camera.frame.h;
	}
	else
	{
		width = m_levelManager.levels[this->currentLevel]->width *
			m_levelManager.levels[this->currentLevel]->tileWidth;

		height = m_levelManager.levels[this->currentLevel]->height *
			m_levelManager.levels[this->currentLevel]->tileHeight;
	}

	this->worldBounds.x = 0;
	this->worldBounds.y = 0;
	this->worldBounds.w = width;
	this->worldBounds.h = height;
	this->worldGridSize.w = m_levelManager.levels[this->currentLevel]->tileWidth;
	this->worldGridSize.h = m_levelManager.levels[this->currentLevel]->tileHeight;

}



