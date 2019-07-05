#include "game.h"
#include "PlayerObject.h"
#include "TextObject.h"
#include "WorldObject.h"
#include "LevelManager.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "DynamicTextManager.h"
#include "GameObject.h"
#include "Util.h"
#include "Camera.h"
#include "Weapon.h"

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
		//Init font library
		TTF_Init();

		//Init Game State
		this->gameState = this->PLAY;

		//Create the game window
		pWindow = SDL_CreateWindow(this->gameTitle.c_str(),
			this->windowXpos,
			this->windowYPos,
			this->camera.frame.w,
			this->camera.frame.h,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

		//Initialize the texture manager
		this->textureManager.init(pWindow);

		// Construct a physics world object, which will hold and simulate the physics objects.
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
		this->gameObjectManager.init();

		//Set the mouse mode
		SDL_ShowCursor(false);
		SDL_SetRelativeMouseMode(SDL_TRUE);

		//Build the world for a particular level
		this->buildWorld("TX_LEVEL1");

		//Initialize the clock object
		this->clock.init();

		//initialize settings menu
		this->settings.init();

	}

	//Create the main player object
	PlayerObject* player = new PlayerObject("GINA_64", 10, 10, 0);

	this->player = make_unique<PlayerObject>(*player);

	this->player->direction = 0;
	this->player->strafe = 0;
	this->player->currentAnimationState = "IDLE";
	// Add a weapon that will have bullet origin that is located half way
	// in the X position and halfway in the Y position from this objects origin
	this->player->addWeapon("BULLET1", .50, .50);

	//set camera to center on player object
	this->camera.setPosition((this->player->physicsBody->GetPosition().x *  this->config.scaleFactor) -
		(camera.frame.w / 2),
		(this->player->physicsBody->GetPosition().y *  this->config.scaleFactor) -
		(camera.frame.h / 2));


	//CREATE A TEST TEXT ITEM
	TextObject* textObject = new TextObject("FPS_LABEL", 1, 0, 0);
	this->addGameObject(textObject);
	//CREATE A DYNAMIC TEST TEXT ITEM
	this->dynamicTextManager.updateText("FPS_VALUE", "this is text");
	textObject = new TextObject("FPS_VALUE", 1, 1, 0);
	this->addGameObject(textObject);



	//CREATE A TEST TEXT ITEM
//	textObject = new TextObject("BULLET_LOCATION_LABEL", 2, 0, 0);
	//this->addGameObject(textObject);
	//CREATE A DYNAMIC TEST TEXT ITEM
	this->dynamicTextManager.updateText("BULLETX", "this is text");
	textObject = new TextObject("BULLETX", 1, 2, 0);
	this->addGameObject(textObject);
	this->dynamicTextManager.updateText("BULLETY", "this is text");
	textObject = new TextObject("BULLETY", 1, 3, 0);
	this->addGameObject(textObject);

	//CREATE A TEST ITEM
	//GameObject* testObject = new GameObject("WALL_BRICK_1", 1, 1, 0);
	//this->addGameObject(testObject);




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

		char buffer[256]; sprintf_s(buffer, "%06d", this->clock.fps);
		string fps(buffer);

		this->dynamicTextManager.updateText("FPS_VALUE", fps);

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
	this->camera.setPosition((this->player->physicsBody->GetPosition().x *  this->config.scaleFactor) -
		(camera.frame.w / 2),
		(this->player->physicsBody->GetPosition().y *  this->config.scaleFactor) -
		(camera.frame.h / 2));

	//Update all of the other non player related update chores for each game object
	this->awakeCount=0;
	for (auto & gameObject : gameObjects)
	{
		gameObject->update();
	}
	
	//Step the box2d physics world
	this->physicsWorld->Step(this->timeStep, this->velocityIterations, this->positionIterations);

}

void Game::render() {

	//Clear teh graphics display
	this->textureManager.clear();

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
	this->textureManager.present();

}

void Game::addGameObject(GameObject* gameObject)
{
	this->gameObjects.push_back(make_unique<GameObject>(*gameObject));


}

void Game::addGameObject(WorldObject* gameObject)
{
	//this->gameObjects.push_back(unique_ptr<WorldObject>(gameObject));
	this->gameObjects.push_back(make_unique<WorldObject>(*gameObject));
}

void Game::addGameObject(TextObject* gameObject)
{
	//this->gameObjects.push_back(unique_ptr<WorldObject>(gameObject));
	this->gameObjects.push_back(make_unique<TextObject>(*gameObject));
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
			std::cout << "FPS is " << this->clock.fps << "\n";
			std::cout << "bodycount is " << Game::gameObjectManager.box2dBodyCount << "\n";
			std::cout << "gameobject count is " << Game::gameObjects.size() << "\n";
			break;
		default:
			break;
		}
	}
}

void Game::buildWorld(string levelId)
{
	//load all of the information needed to build the level
	this->levelManager.loadLevel("TX_LEVEL1");

	//Initialize world bounds and gridsize based on current level loaded info
	this->initWorldBounds();

	//Init Camera
	this->camera.init(&this->worldBounds);

	//Build the actual level gameobjects
	this->buildLevel("TX_LEVEL1");

}



void Game::buildLevel(string levelId)
{
	Level* level = this->levelManager.levels[levelId];
	LevelObject* levelObject;
	//unique_ptr<WorldObject> worldObject;
	WorldObject* gameObject;


	for (int y = 0; y < level->height; y++)
	{
		for (int x = 0; x < level->width; x++)
		{

			if (level->levelObjects[x][y].gameObjectId.empty() == false)
			{
				levelObject = &level->levelObjects[x][y];
				gameObject = new WorldObject(levelObject->gameObjectId, x, y, levelObject->angleAdjustment);

				this->addGameObject(gameObject);

			}

		}
	}
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
	this->worldGridSize.w = this->levelManager.levels[this->currentLevel]->tileWidth;
	this->worldGridSize.h = this->levelManager.levels[this->currentLevel]->tileHeight;

}

Game::~Game()
{

	printf("cleaning game\n");

	//Delete SDL stuff
	SDL_DestroyWindow(this->pWindow);
	SDL_Quit();
	TTF_Quit();

	this->gameObjects.clear();

	//Delete box2d world - should delete all bodies and fixtures within
	delete this->physicsWorld;

}

Game::Game()
{

	

}


