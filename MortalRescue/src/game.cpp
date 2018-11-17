
#include <SDL.h>
#include <SDL_image.h>
#include <json/json.h>
#include <iostream>
#include <fstream>
#include "game.h"
#include <stdio.h>
using namespace std;


bool Game::init()
{
	std::cout << "Hello World!\n";

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
		textureManager.init(pWindow);
		m_bRunning = true;
	}
	return true;
}

void Game::update() {

	//printf("updating game\n");

}



void Game::render() {

	this->textureManager.draw();
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

