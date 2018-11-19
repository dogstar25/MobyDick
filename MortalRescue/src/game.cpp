
#include "game.h"

bool Game::init()
{
	//Get all of the configuration values
	getConfig();

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
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

		//Initilaze the Game Object Manager
		//This will hold all possible game objects that the game/level supports
		this->gameObjectManager.init(&this->textureManager);

		//Set the main player gameObject as defined by config
		this->player = this->gameObjectManager.getGameObject(this->playerGameObjectId);
		m_bRunning = true;
	}
	return true;
}

void Game::update() {

	//printf("updating game\n");

}


void Game::render() {

	//NOTE: This will be looping thru all of the game objects that need to be rendered
	this->textureManager.render(this->player);
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

	return true;
}

void Game::handleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		case SDL_KEYDOWN:
			m_bRunning = false;
			break;

		default:
			break;
		}
	}
}

