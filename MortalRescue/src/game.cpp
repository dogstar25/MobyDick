
#include "game.h"

Clock Game::clock;
Util Game::util;
TextureManager Game::textureManager;
GameObjectManager Game::gameObjectManager;
Config Game::config;
using namespace chrono_literals;

bool Game::init()
{

	//Get all of the configuration values
	getConfig();

	//Initialize world
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) 
	{
		printf("SDL_Init success\n");

		//Create the game window
		pWindow = SDL_CreateWindow(this->windowTitle.c_str(),
			this->windowXpos,
			this->windowYPos,
			this->screenWidth, 
			this->screenHeight, 
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

		//Initialize the textture manager
		this->textureManager.init(pWindow);

		// Construct a world object, which will hold and simulate the rigid bodies.
		this->physicsWorld = new b2World(this->gravity);

		//Initilaze the Game Object Manager
		//This will hold all possible game objects that the game/level supports
		this->gameObjectManager.init();

		bRunning = true;

	}

	//Initialize the clock object
	clock.init();


	//Add a few objects to the world
	GameObject *gameObject=nullptr;
	gameObject = Game::gameObjectManager.buildGameObject(this->playerGameObjectId, this->physicsWorld);
	this->gameObjects.push_back(*gameObject);
	this->player = gameObject; //THIS IS THE MAIN PLAYER

	gameObject = Game::gameObjectManager.buildGameObject("ROCK", this->physicsWorld);
	this->gameObjects.push_back(*gameObject);

	gameObject = Game::gameObjectManager.buildGameObject("GROUND1", this->physicsWorld);
	this->gameObjects.push_back(*gameObject);

	gameObject = Game::gameObjectManager.buildGameObject("ROCK", this->physicsWorld);
	this->gameObjects.push_back(*gameObject);

	gameObject = Game::gameObjectManager.buildGameObject("SWORDLADY", this->physicsWorld);
	gameObject->currentAnimationState = "IDLE";
	this->gameObjects.push_back(*gameObject);

	return true;
}

void Game::update() {

	//Specifiaclly handle input and stuff for the one player gameObject
	this->player->updatePlayer();

	//Update all of the other none player related update chores for each game object
	for (auto & gameObject : gameObjects) {
		gameObject.update();
	}

	/*Temp code for physics updating*/
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	this->physicsWorld->Step(timeStep, velocityIterations, positionIterations);
	
}


void Game::render() {

	Game::textureManager.clear();
	//This will be looping thru all of the game objects that need to be rendered
	for (auto & gameObject : gameObjects) {
		Game::textureManager.render(&gameObject);
	}

	Game::textureManager.present();

}

void Game::clean() {
	printf("cleaning game\n");
	SDL_DestroyWindow(this->pWindow);
	SDL_Quit();
}

bool Game::getConfig()
{
	//Read file and stream it to a JSON object
	Json::Value root;
	ifstream ifs("config/gameConfig.json");
	ifs >> root;

	//Get and store config values
	this->gameTitle = root["gameTitle"].asString();
	this->windowTitle = root["windowSettings"]["title"].asString();
	this->screenWidth = root["windowSettings"]["width"].asInt();
	this->screenHeight = root["windowSettings"]["height"].asInt();
	this->playerGameObjectId = root["playerGameObjectId"].asString();
	this->gravity.Set(root["physics"]["gravity"]["x"].asInt(), root["physics"]["gravity"]["y"].asFloat());
	this->config.scaleFactor = root["physics"]["box2dScale"].asFloat();

	return true;
}

void Game::handleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			bRunning = false;
			break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			if ((char)event.key.keysym.sym == SDLK_ESCAPE)
			{
				event.type = SDL_QUIT;
				SDL_PushEvent(&event);
			}
			else
			{
				this->player->handlePlayerMovementEvent(&event);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			this->testBlocks(&event, this->physicsWorld);
			break;

		default:
			break;
		}
	}
}

void Game::testBlocks(SDL_Event* event, b2World* physicsWorld)
{

	std::cout << "Object created " << " \n";
	std::cout << "X " << event->button.x << " \n";
	std::cout << "Y " << event->button.y << " \n";

	GameObject* gameObject;
	gameObject = new GameObject();

	//build id
	int count = this->gameObjects.size();
	string id = "block" + to_string(count);
	gameObject->definition.id = id;

	gameObject->definition.description = "block";
	//gameObject->xSize = Game::util.generateRandomNumber(1,30) * .1;
	//gameObject->ySize = Game::util.generateRandomNumber(1, 30) * .1;
	gameObject->definition.xSize = .3;
	gameObject->definition.ySize = .3;

	gameObject->definition.initPosX = event->button.x / Game::config.scaleFactor;
	gameObject->definition.initPosY = event->button.y / Game::config.scaleFactor;

	//gameObject->isPrimitiveShape = true;
	//gameObject->primativeColor = Game::util.generateRandomColor();

	string textureId = "TX_TILE1";
	gameObject->staticTexture = Game::textureManager.getTexture(textureId);

	gameObject->definition.isPhysicsObject = true;
	gameObject->definition.physicsType = "B2_DYNAMIC";
	gameObject->definition.friction = .3;
	gameObject->definition.density = 2;
	gameObject->definition.linearDamping = .2;
	gameObject->definition.angularDamping = .2;
	gameObject->physicsBody = Game::gameObjectManager.buildB2Body(&gameObject->definition, physicsWorld);

	this->gameObjects.push_back(*gameObject);

}






