
#include <iostream>
#include <fstream>
#include "game.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
using namespace rapidjson;
using namespace std;


SDL_Texture* playerTex;
SDL_Rect srcRect, destRect;

bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	std::cout << "Hello World!\n";

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		printf("SDL_Init success\n");

		m_pWindow = SDL_CreateWindow(title,
			xpos, ypos,
			width, height,
			flags);

		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
		SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);

		m_bRunning = true;

		//Initialize player
		SDL_Surface* tempSurface = IMG_Load("assets/player.png");
		playerTex = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
		SDL_FreeSurface(tempSurface);

		//Test reading in a json file
		ifstream ifs("assets/gameObjectAssets.json");
		IStreamWrapper isw(ifs);
		Document d;
		d.ParseStream(isw);

		string test = d["description"].GetString();


	}
	return true;
}

void Game::update() {

	//printf("updating game\n");

}



void Game::render() {
	SDL_RenderClear(m_pRenderer);

	//test
	SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);

	destRect.h = 64;
	destRect.w = 64;

	SDL_RenderCopy(m_pRenderer, playerTex, NULL, &destRect);





	SDL_RenderPresent(m_pRenderer);
}

void Game::clean() {
	printf("cleaning game\n");
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);

	SDL_Quit();
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

