#include "Game.h"


Game::~Game()
{

	delete m_contactListener;
	delete m_contactFilter;
}

bool Game::init()
{

	return true;
}


bool Game::init(ContactListener* contactListener, ContactFilter* contactFilter, 
	ComponentFactory* componentFactory)
{
	m_contactListener = contactListener;
	m_contactFilter = contactFilter;
	m_componentFactory = componentFactory;

	return true;
}


void Game::play()
{

}


void Game::_displayLoadingMsg()
{
	static int statusDots{};
	std::string statusMsg{"Loading"};

	statusDots++;
	if (statusDots > 4) {
		statusDots = 1;
	}

	for (int x = 0; x < statusDots;x++) {

		statusMsg += ".";
	}

	//Assume nothing has been initialzed yet except for the renderer so build and render a text item in
	//a very crude and manual way
	Renderer::instance().clear();
	TTF_Font* m_fontObject = TTF_OpenFont("assets/arial.ttf", 32);
	SDL_Surface* tempSurface = TTF_RenderText_Blended(m_fontObject, statusMsg.c_str(), SDL_Color(255, 255, 255, 255));
	SDL_Texture* sdlTexture = Renderer::instance().createTextureFromSurface(tempSurface);
	TTF_CloseFont(m_fontObject);
	SDL_Rect dest = {
		GameConfig::instance().windowWidth() / 2 - 100,
		GameConfig::instance().windowHeight() / 2 - 42,
		tempSurface->w, tempSurface->h };
	SDL_RenderCopy(Renderer::instance().SDLRenderer(), sdlTexture, nullptr, &dest);
	Renderer::instance().present();
	SDL_DestroyTexture(sdlTexture);


}