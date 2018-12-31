
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

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) 
	{
		printf("SDL_Init success\n");

		//Create the game window
		pWindow = SDL_CreateWindow(this->windowTitle.c_str(),
			this->windowXpos,
			this->windowYPos,
			this->screenWidth, 
			this->screenHeight, 
			this->windowFlags);

		//Initialize the textture manager
		this->textureManager.init(pWindow);

		// Construct a world object, which will hold and simulate the rigid bodies.
		this->physicsWorld = new b2World(this->gravity);

		//Initilaze the Game Object Manager
		//This will hold all possible game objects that the game/level supports
		this->gameObjectManager.init(&this->textureManager, this->physicsWorld);

		//Set the main player gameObject as defined by config
		this->player = this->gameObjectManager.getGameObject(this->playerGameObjectId);
		this->player->xPos = 0;
		this->player->yPos = 0;
		
		bRunning = true;


		//
		//Temp Code
		//
		// Define the ground body.
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0.0f, 30.0f);

		// Call the body factory which allocates memory for the ground body
		// from a pool and creates the ground box shape (also from a pool).
		// The body is also added to the world.
		b2Body* groundBody = this->physicsWorld->CreateBody(&groundBodyDef);

		// Define the ground box shape.
		b2PolygonShape groundBox;

		// The extents are the half-widths of the box.
		groundBox.SetAsBox(500.0f, 10.0f);

		// Add the ground fixture to the ground body.
		groundBody->CreateFixture(&groundBox, 0.0f);



	}

	//Initialize the clock object
	clock.init();

	return true;
}

void Game::update() {

	this->player->update();

	/*Temp code for physics test*/
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	this->physicsWorld->Step(timeStep, velocityIterations, positionIterations);
	//this->physicsWorld->ClearForces(); 
	
}


void Game::render() {

	this->textureManager.clear();
	//NOTE: This will be looping thru all of the game objects that need to be rendered
	//this->textureManager.render(this->player);
	for (auto & gameObject : gameObjectManager.gameObjectMap) {
		this->textureManager.render(&gameObject.second);
	}
	
	this->textureManager.present();

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
				this->player->handleEvent(&event);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			this->gameObjectManager.testBlocks(&event, this->physicsWorld);
			break;

		default:
			break;
		}
	}
}






