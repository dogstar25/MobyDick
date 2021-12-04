#include "Game.h"
#include "GameConfig.h"

#include "RendererSDL.h"
#include "opengl/GLRenderer.h"


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
	ComponentFactory* componentFactory, StatusManager* statusManager)
{
	m_contactListener = contactListener;
	m_contactFilter = contactFilter;
	m_componentFactory = componentFactory;
	m_statusMananger = statusManager;

	if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

		m_renderer = std::make_shared<GLRenderer>(GLRenderer());

	}
	else if (GameConfig::instance().rendererType() == RendererType::SDL) {

		m_renderer = std::make_shared<RendererSDL>(RendererSDL());

	}

	return true;
}


void Game::play()
{
	while (m_gameState != GameState::QUIT) {

		std::optional<SceneAction> action = SceneManager::instance().pollEvents();

		if (action.has_value()) {
			if (action->actionCode == SCENE_ACTION_QUIT) {
				m_gameState = GameState::QUIT;
			}
			else if (action->actionCode == SCENE_ACTION_EXIT) {
				SceneManager::instance().popScene();
			}
			else if (action->actionCode == SCENE_ACTION_ADD) {
				SceneManager::instance().pushScene(action->sceneId);
			}
			else if (action->actionCode == SCENE_ACTION_REPLACE) {
				SceneManager::instance().popScene();
				SceneManager::instance().pushScene(action->sceneId);
			}
		}

		SceneManager::instance().run();
	}
}


void Game::_displayLoadingMsg()
{
	//temp
	//
	return;

	static int statusDots{};
	std::string statusMsg{"Loading"};
	Texture texture{};

	statusDots++;
	if (statusDots > 4) {
		statusDots = 1;
	}

	for (int x = 0; x < statusDots;x++) {

		statusMsg += ".";
	}

	//Assume nothing has been initialzed yet except for the renderer so build and render a text item in
	//a very crude and manual way
	m_renderer->clear();
	TTF_Font* m_fontObject = TTF_OpenFont("assets/arial.ttf", 32);
	SDL_Surface* tempSurface = TTF_RenderText_Blended(m_fontObject, statusMsg.c_str(), SDL_Color(255, 255, 255, 255));
	SDL_Texture* sdlTexture = m_renderer->createTextureFromSurface(tempSurface);
	TTF_CloseFont(m_fontObject);
	SDL_FRect dest = {
		GameConfig::instance().windowWidth() / (float)2 - (float)100,
		GameConfig::instance().windowHeight() / (float)2 - (float)42,
		(float)tempSurface->w, (float)tempSurface->h };

	texture.sdlTexture = sdlTexture;
	texture.surface = tempSurface;

	m_renderer->drawSprite(dest, SDL_Color{ 255,255,255,255 }, LAYER_MENU, &texture, nullptr, 0, false, SDL_Color{});
	m_renderer->present();
	SDL_DestroyTexture(sdlTexture);

}