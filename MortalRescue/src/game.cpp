#include "game.h"
#include "PlayerObject.h"
#include "LevelManager.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Util.h"
#include "Camera.h"


Clock Game::clock;
Util Game::util;
TextureManager Game::textureManager;
GameObjectManager Game::gameObjectManager;
LevelManager Game::levelManager;
Config Game::config;
Camera Game::camera;
SDL_Rect Game::worldBounds;

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
		pWindow = SDL_CreateWindow(this->gameTitle.c_str(),
			this->windowXpos,
			this->windowYPos,
			this->camera.frame.w, 
			this->camera.frame.h, 
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

		//Init Camera
		this->camera.init(&this->worldBounds);

		//Initialize the textture manager
		this->textureManager.init(pWindow);

		// Construct a world object, which will hold and simulate the rigid bodies.
		this->physicsWorld = new b2World(this->gravity);

		//Initilaze the Game Object Manager
		//This will hold all possible game objects that the game/level supports
		this->gameObjectManager.init();

		//Create the player object
		/*
		PlayerObject* playerObject = (PlayerObject*)Game::gameObjectManager.buildGameObject("PISTOLLADY", this->physicsWorld);
		playerObject->currentAnimationState = "RUN";
		this->player = (PlayerObject*)playerObject;
		this->player->direction = 0;
		this->player->strafe = 0;
		*/
		
		PlayerObject* playerObject = 
			(PlayerObject*)Game::gameObjectManager.buildGameObject("SPACESHIP1", this->physicsWorld, 5, 5);

		this->player = (PlayerObject*)playerObject;
		this->player->direction = 0;
		this->player->strafe = 0;
		playerObject->currentAnimationState = "RUN";
		
		//SDL_ShowCursor(false);

		//set camera to center on player object
		this->camera.setPosition((this->player->physicsBody->GetPosition().x *  Game::config.scaleFactor) -
									(camera.frame.w / 2),
								 (this->player->physicsBody->GetPosition().y *  Game::config.scaleFactor) -
									(camera.frame.h / 2));

		bRunning = true;

	}

	//Initialize the clock object
	clock.init();

	//Load level 1
	Game::levelManager.loadLevel("TX_LEVEL1");
	this->buildLevel("TX_LEVEL1");
	
	//Add a few objects to the world
	
	GameObject *gameObject=nullptr;
	gameObject = Game::gameObjectManager.buildGameObject("BOWMAN", this->physicsWorld, 1, 1);
	this->gameObjects.push_back(*gameObject);
	/*
	gameObject = Game::gameObjectManager.buildGameObject("ROCK", this->physicsWorld);
	this->gameObjects.push_back(*gameObject);

	gameObject = Game::gameObjectManager.buildGameObject("GROUND1", this->physicsWorld);
	this->gameObjects.push_back(*gameObject);

	gameObject = Game::gameObjectManager.buildGameObject("ROCK", this->physicsWorld);
	this->gameObjects.push_back(*gameObject);
	*/
	gameObject = Game::gameObjectManager.buildGameObject("SWORDLADY", this->physicsWorld, 2, 2);
	gameObject->currentAnimationState = "IDLE";
	this->gameObjects.push_back(*gameObject);
	
	return true;
}

void Game::update() {

	//Specifiaclly handle input and stuff for the one player gameObject
	this->player->updatePlayer();

	//Update the camera frame to point to the new player position
	this->camera.setPosition((this->player->physicsBody->GetPosition().x *  Game::config.scaleFactor) -
		(camera.frame.w / 2),
		(this->player->physicsBody->GetPosition().y *  Game::config.scaleFactor) -
		(camera.frame.h / 2));

	//Update all of the other non player related update chores for each game object
	this->awakeCount=0;
	for (auto & gameObject : gameObjects) {

		//count the number of awake objects - later to be used to adjust the players objects force/velocity
		if (gameObject.physicsBody->IsAwake()) {
			this->awakeCount++;
		}

		gameObject.update();
	}
	
	//cout << "awake count " << this->awakeCount << "\n";

	this->physicsWorld->Step(this->timeStep, this->velocityIterations, this->positionIterations);

}

void Game::render() {

	Game::textureManager.clear();

	//render the player
	Game::textureManager.render(this->player);
	
	//Render all of the game objects
	for (auto & gameObject : gameObjects) {
		Game::textureManager.render(&gameObject);
	}

	Game::textureManager.present();

}

void Game::clean() {
	printf("cleaning game\n");
	//SDL_SetWindowFullscreen(pWindow, SDL_WINDOW_RESIZABLE);
	SDL_DestroyWindow(this->pWindow);
	SDL_Quit();
}

bool Game::getConfig()
{
	//Read file and stream it to a JSON object
	Json::Value root;
	ifstream ifs("assets/gameConfig.json");
	ifs >> root;

	//Get and store config values
	this->gameTitle = root["gameTitle"].asString();
	this->gravity.Set(root["physics"]["gravity"]["x"].asInt(), root["physics"]["gravity"]["y"].asFloat());

	this->timeStep = root["physics"]["timeStep"].asFloat();
	this->velocityIterations = root["physics"]["velocityIterations"].asInt();
	this->positionIterations = root["physics"]["positionIterations"].asInt();

	this->config.scaleFactor = root["physics"]["box2dScale"].asFloat();
	this->config.mouseSensitivity = root["mouseSensitivity"].asFloat();

	this->worldBounds.x = 0;
	this->worldBounds.y = 0;
	this->worldBounds.w = root["world"]["width"].asInt();
	this->worldBounds.h = root["world"]["height"].asInt();
	this->camera.frame.w = root["camera"]["width"].asInt();
	this->camera.frame.h = root["camera"]["height"].asInt();

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
		case SDL_MOUSEMOTION:
			this->player->handlePlayerMovementEvent(&event);
			break;

		default:
			break;
		}
	}
}

void Game::addToActiveGameOjectArray(GameObject* gameObject)
{
	this->gameObjects.push_back(*gameObject);
}

void Game::buildLevel(string levelId)
{

	Level* level = Game::levelManager.levels[levelId];
	LevelObject* levelObject;
	GameObject* gameObject;

	for (int y = 0; y < level->height; y++)
	{
		for (int x = 0; x < level->width; x++)
		{

			if (level->levelObjects[x][y].gameObjectId.empty() == false)
			{
				levelObject = &level->levelObjects[x][y];
				gameObject = Game::gameObjectManager.buildGameObject(levelObject->gameObjectId, physicsWorld,
					x, y, levelObject->angleAdjustment);

				this->gameObjects.push_back(*gameObject);
			}

		}
	}
}


void Game::testBlocks(SDL_Event* event, b2World* physicsWorld)
{

	//std::cout << "Object created " << " \n";
	//std::cout << "X " << event->button.x << " \n";
	//std::cout << "Y " << event->button.y << " \n";

	GameObject* gameObject;
	gameObject = new GameObject();
	gameObject->definition = new GameObjectDefinition();

	//build id
	int count = this->gameObjects.size();
	string id = "block" + to_string(count);
	gameObject->definition->id = id;

	gameObject->definition->description = "block";
	//gameObject->xSize = Game::util.generateRandomNumber(1,30) * .1;
	//gameObject->ySize = Game::util.generateRandomNumber(1, 30) * .1;
	gameObject->definition->xSize = 1;
	gameObject->definition->ySize = 1;

	gameObject->definition->initPosX = event->button.x / Game::config.scaleFactor;
	gameObject->definition->initPosY = event->button.y / Game::config.scaleFactor;

	//gameObject->isPrimitiveShape = true;
	//gameObject->primativeColor = Game::util.generateRandomColor();

	string textureId = "TX_TILE1";
	gameObject->staticTexture = Game::textureManager.getTexture(textureId)->texture;

	gameObject->definition->isPhysicsObject = true;
	gameObject->definition->physicsType = "B2_STATIC";
	//gameObject->definition.physicsType = "B2_DYNAMIC";
	gameObject->definition->friction = .5;
	gameObject->definition->density = 10.0;
	gameObject->definition->linearDamping = 0;
	gameObject->definition->angularDamping = 0;
	gameObject->physicsBody = Game::gameObjectManager.buildB2Body(gameObject->definition, physicsWorld);

	this->gameObjects.push_back(*gameObject);

}






