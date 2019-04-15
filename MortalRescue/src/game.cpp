#include "game.h"
#include "PlayerObject.h"
#include "WorldObject.h"
#include "LevelManager.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Util.h"
#include "Camera.h"
#include "Weapon.h"
//#include "vld.h"



Clock Game::clock;
Util Game::util;
TextureManager Game::textureManager;
GameObjectManager Game::gameObjectManager;
LevelManager Game::levelManager;
Config Game::config;
Camera Game::camera;
SDL_Rect Game::worldBounds;
b2World* Game::physicsWorld;
vector<unique_ptr<GameObject>> Game::gameObjects;

using namespace chrono_literals;

/*
Initialize Game
*/
bool Game::init()
{

	//Get all of the configuration values
	getConfig();

	//Initialize world
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		//Init Game State
		this->gameState = this->PLAY;

		//Create the game window
		pWindow = SDL_CreateWindow(this->gameTitle.c_str(),
			this->windowXpos,
			this->windowYPos,
			this->camera.frame.w,
			this->camera.frame.h,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

		//Initialize the textture manager
		this->textureManager.init(pWindow);

		// Construct a world object, which will hold and simulate the rigid bodies.
		this->physicsWorld = new b2World(this->gravity);
		this->physicsWorld->SetAutoClearForces(true);
		//Add a collision contact listener
		this->physicsWorld->SetContactListener(&this->gameObjectContactListner);

		//Debug Mode
		if (this->debugDrawMode == true)
		{
			this->debugDraw.SetFlags(DebugDraw::e_shapeBit);
			this->physicsWorld->SetDebugDraw(&this->debugDraw);
		}

		//Initilaze the Game Object Manager
		//This will hold all possible game objects that the game/level supports
		this->gameObjectManager.init();

		//Create the main player object
		//this->player =
		//	dynamic_cast<unique_ptr<PlayerObject>>(Game::gameObjectManager.buildGameObject("GINA_64", GameObjectType::PLAYER_OBJECT, 5, 5));
		//this->player = make_unique<PlayerObject>(Game::gameObjectManager.buildGameObject("GINA_64", GameObjectType::PLAYER_OBJECT, 5, 5));
		PlayerObject* test = dynamic_cast<PlayerObject*>(Game::gameObjectManager.buildGameObject("GINA_64", GameObjectType::PLAYER_OBJECT, 5, 5));
		//this->player = make_unique<PlayerObject>(*test);
		this->player = unique_ptr<PlayerObject>(test);

		this->player->direction = 0;
		this->player->strafe = 0;
		this->player->currentAnimationState = "IDLE";
		// Add a weapon that will have bullet origin that is located half way
		// in the X position and halfway in the Y position from this objects origin
		this->player->addWeapon("BULLET1", .50, .50);

		//Set the mouse mode
		SDL_ShowCursor(false);
		SDL_SetRelativeMouseMode(SDL_TRUE);

		//Load level 1
		Game::levelManager.loadLevel("TX_LEVEL1");
		this->buildLevel("TX_LEVEL1");

		this->initWorldBounds();

		//Init Camera
		this->camera.init(&this->worldBounds);

		//set camera to center on player object
		this->camera.setPosition((this->player->physicsBody->GetPosition().x *  Game::config.scaleFactor) -
			(camera.frame.w / 2),
			(this->player->physicsBody->GetPosition().y *  Game::config.scaleFactor) -
			(camera.frame.h / 2));

		//Initialize the clock object
		clock.init();

		//initialize settings menu
		this->settings.init();
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
	clock.tick();

	//Only update and render if we have passed the 60 fps time passage
	if (clock.gameloop_time_accum >= milisecsPerUpdate)
	{
		//Handle updating objects positions and physics
		update();

		//render everything
		render();

		//Increment frame counter and calculate FPS and reset the gameloop timer
		clock.current_frame_cnt++;
		clock.calcFps();
		clock.resetGameLoopTimeAccum();
	}

}

/*
Settings Menus
*/
void Game::settingsMenu()
{
	this->settings.run();

	this->gameState = PLAY;
	
}


void Game::update() {

	//Specifiaclly handle input and stuff for the one player gameObject
	this->player->update();

	//Update the camera frame to point to the new player position
	this->camera.setPosition((this->player->physicsBody->GetPosition().x *  Game::config.scaleFactor) -
		(camera.frame.w / 2),
		(this->player->physicsBody->GetPosition().y *  Game::config.scaleFactor) -
		(camera.frame.h / 2));

	//Update all of the other non player related update chores for each game object
	this->awakeCount=0;
	for (auto & gameObject : gameObjects)
	{

		gameObject->update();
	}
	
	this->physicsWorld->Step(this->timeStep, this->velocityIterations, this->positionIterations);

}

void Game::render() {

	//Clear teh graphics display
	Game::textureManager.clear();

	//render the player
	this->player->render();
	
	//Render all of the game objects
	for (auto & gameObject : gameObjects) {
		gameObject->render();
	}

	//DebugDraw
	if (this->debugDrawMode == true)
	{
		this->physicsWorld->DrawDebugData();
	}

	//Push all drawn things to the graphics display
	Game::textureManager.present();

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
	this->debugDrawMode = root["physics"]["debugDrawMode"].asBool(); 

	this->config.scaleFactor = root["physics"]["box2dScale"].asFloat();
	this->config.mouseSensitivity = root["mouseSensitivity"].asFloat();

	this->camera.frame.w = root["camera"]["width"].asInt();
	this->camera.frame.h = root["camera"]["height"].asInt();

	return true;
}

void Game::handleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {

		switch (event.type) 
		{
		case SDL_QUIT:
			this->gameState = QUIT;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		case SDL_MOUSEMOTION:
			if ((char)event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN)
			{
				this->gameState = SETTINGS;
			}
			else
			{
				this->player->handlePlayerMovementEvent(&event);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			//this->testBlocks(&event, this->physicsWorld);
			this->player->weapon->fire();
			std::cout << "FPS is " << Game::clock.fps << "\n";
			std::cout << "bodycount is " << Game::gameObjectManager.box2dBodyCount << "\n";
			std::cout << "gameobject count is " << Game::gameObjects.size() << "\n";
			break;
		default:
			break;
		}
	}
}

void Game::buildLevel(string levelId)
{
	this->currentLevel = levelId;
	Level* level = Game::levelManager.levels[levelId];
	LevelObject* levelObject;
	//unique_ptr<WorldObject> worldObject;
	WorldObject* worldObject;


	for (int y = 0; y < level->height; y++)
	{
		for (int x = 0; x < level->width; x++)
		{

			if (level->levelObjects[x][y].gameObjectId.empty() == false)
			{
				levelObject = &level->levelObjects[x][y];
				worldObject = dynamic_cast<WorldObject*>(
					Game::gameObjectManager.buildGameObject(levelObject->gameObjectId, GameObjectType::WORLD_OBJECT,
					x, y, levelObject->angleAdjustment));

				//Use the first level object found to determine and store the tile width and height for the map
				if (level->tileHeight == 0 and level->tileWidth == 0)
				{
					level->tileWidth = worldObject->definition->xSize * Game::config.scaleFactor;
					level->tileHeight = worldObject->definition->ySize * Game::config.scaleFactor;
				}

				this->gameObjects.push_back(unique_ptr<WorldObject>(worldObject));

				//this->player = unique_ptr<PlayerObject>(move(test));


			}

		}
	}

	int todd = 1;
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
		width = this->levelManager.levels[this->currentLevel]->width *
			this->levelManager.levels[this->currentLevel]->tileWidth;

		height = this->levelManager.levels[this->currentLevel]->height *
			this->levelManager.levels[this->currentLevel]->tileHeight;
	}

	this->worldBounds.x = 0;
	this->worldBounds.y = 0;
	this->worldBounds.w = width;
	this->worldBounds.h = height;

}

Game::~Game()
{

	printf("cleaning game\n");

	//Delete SDL stuff
	SDL_DestroyWindow(this->pWindow);
	SDL_Quit();

	this->gameObjects.clear();

	//Delete box2d world - should delete all bodies and fixtures within
	delete this->physicsWorld;

}

Game::Game()
{

	

}


