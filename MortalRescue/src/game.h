#ifndef __Game__
#define __Game__

#include <string>
#include "TextureManager.h"

class Game {
public:
	Game() {}
	~Game() {}

	bool init();

	void render();
	void update();
	void handleEvents();
	void clean();

	bool running() { return m_bRunning; }

private:
	//Game Window Settings
	//string windowTitle;
	std::string windowTitle;
	std::string gameTitle;
	int screenWidth, screenHeight;
	Uint32 windowXpos= SDL_WINDOWPOS_CENTERED, windowYPos= SDL_WINDOWPOS_CENTERED;
	Uint32 windowFlags= SDL_WINDOW_RESIZABLE;

	TextureManager textureManager;
		
	SDL_Window* pWindow;
	bool m_bRunning;

	bool getConfig();
};

#endif

