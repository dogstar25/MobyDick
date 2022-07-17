#include "Game.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "IMGui/IMGuiUtil.h"
#include <memory>



Game::~Game()
{

	ImGui::DestroyContext();

	// These shutdowns no longer needed?
	//ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplSDL2_Shutdown();

}

bool Game::init(std::shared_ptr<ContactListener> contactListener, std::shared_ptr<ContactFilter> contactFilter,
	std::shared_ptr<ComponentFactory> componentFactory, std::shared_ptr<ActionFactory> actionFactory, std::shared_ptr<ParticleEffectsFactory> particleEffectsFactory,
	std::shared_ptr<CutSceneFactory> cutSceneFactory, std::shared_ptr<IMGuiFactory> iMGuiFactory, std::shared_ptr<TriggerFactory> triggerFactory, std::shared_ptr<ContextManager> contextManager)
{

	//Set all of our game specific factories and managers
	m_contactListener = contactListener;
	m_contactFilter = contactFilter;
	m_componentFactory = componentFactory;
	m_actionFactory = actionFactory;
	m_particleEffectsFactory = particleEffectsFactory;
	m_cutSceneFactory = cutSceneFactory;
	m_contextMananger = contextManager;
	m_iMGUIFactory = iMGuiFactory;
	m_triggerFactory = triggerFactory;

	//Get all of the configuration values
	GameConfig::instance().init("gameConfig");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		assert(true && "SDL_Init faled!");
	}
	
	std::optional<SDL_Point> gameResolution = _determineScreenResolution();
	if (gameResolution.has_value() == false){
		assert(true && "No Supported screen resolution was detected!");
	}
	else {
		m_gameScreenResolution = gameResolution.value();
	}

	//Create the game window
	uint16 windowFlags = 0 | SDL_WINDOW_OPENGL;
	if (GameConfig::instance().windowFullscreen() == true)
	{
		windowFlags = windowFlags | SDL_WINDOW_FULLSCREEN;
	}
	else
	{
		windowFlags = windowFlags | SDL_WINDOW_RESIZABLE;
	}

	m_window = SDL_CreateWindow(
		GameConfig::instance().gameTitle().c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_gameScreenResolution.x,
		m_gameScreenResolution.y,
		windowFlags);


	if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

		m_renderer = std::make_shared<GLRenderer>(GLRenderer());

	}
	else if (GameConfig::instance().rendererType() == RendererType::SDL) {

		m_renderer = std::make_shared<RendererSDL>(RendererSDL());

	}

	return true;
}


void Game::setWorldParams(SDL_Rect gameWorldBounds, SDL_Point gameTileSize)
{
	m_worldBounds = gameWorldBounds;
	m_worldTileSize = gameTileSize;
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
				SceneManager::instance().pushScene(action->actionId, false);
			}
			else if (action->actionCode == SCENE_ACTION_ADD_AND_PAUSE) {
				SceneManager::instance().pushScene(action->actionId, true);
			}
			else if (action->actionCode == SCENE_ACTION_REPLACE) {
				SceneManager::instance().popScene();
				SceneManager::instance().pushScene(action->actionId);
			}
			else if (action->actionCode == SCENE_ACTION_LOAD_LEVEL) {
				SceneManager::instance().loadLevel(action->actionId);
			}
			else if (action->actionCode == SCENE_ACTION_LOAD_NEXTLEVEL) {
				SceneManager::instance().loadNextLevel();
			}
			else if (action->actionCode == SCENE_ACTION_LOAD_CURRENTLEVEL) {
				SceneManager::instance().loadCurrentLevel();
			}
			else if (action->actionCode == SCENE_ACTION_DIRECT) {
				SceneManager::instance().directScene(action->actionId);
			}
			else if (action->actionCode == SCENE_ACTION_RELEASE_DIRECT) {
				SceneManager::instance().releaseDirectScene();
			}

		}

		SceneManager::instance().run();
	}
}


void Game::_displayLoadingMsg()
{

	static int statusDots{};
	std::string statusMsg{ "Loading" };
	Texture texture{};
	SDL_Texture* sdlTexture{};

	statusDots++;
	if (statusDots > 4) {
		statusDots = 1;
	}

	for (int x = 0; x < statusDots; x++) {

		statusMsg += ".";
	}

	//Assume nothing has been initialzed yet except for the renderer so build and render a text item in
	//a very crude and manual way
	m_renderer->clear();
	TTF_Font* m_fontObject = TTF_OpenFont("assets/fonts/arial.ttf", 32);

	//Create the surface
	SDL_Surface* tempSurface = TTF_RenderText_Blended(m_fontObject, statusMsg.c_str(), SDL_Color(255, 255, 255, 255));
	texture.surface = tempSurface;

	//Do different stuff based on if we're using SDL or direct OpenGL to render
	if (GameConfig::instance().rendererType() == RendererType::SDL) {
		sdlTexture = SDL_CreateTextureFromSurface(m_renderer->sdlRenderer(), tempSurface);
		texture.sdlTexture = sdlTexture;
	}
	else if (GameConfig::instance().rendererType() == RendererType::OPENGL) {
		GL_TextureIndexType textureIndex = GL_TextureIndexType::DYNAMICALLY_LOADED;
		GLuint textureAtlasId = static_cast<GLRenderer*>(renderer())->getTextureId(textureIndex);
		glActiveTexture((int)textureIndex);
		glBindTexture(GL_TEXTURE_2D, textureAtlasId);
		texture.openglTextureIndex = textureIndex;
	}

	SDL_Rect quad = { 0 , 0, tempSurface->w, tempSurface->h };
	texture.textureAtlasQuad = std::move(quad);
	texture.sdlTexture = sdlTexture;
	texture.surface = tempSurface;

	TTF_CloseFont(m_fontObject);
	SDL_FRect dest = {
		m_gameScreenResolution.x / (float)2 - (float)100,
		m_gameScreenResolution.y / (float)2 - (float)42,
		(float)tempSurface->w, (float)tempSurface->h };

	m_renderer->drawSprite(dest, SDL_Color{ 255,255,255,255 }, &texture, &texture.textureAtlasQuad, 0, false, SDL_Color{}, RenderBlendMode::BLEND);
	m_renderer->present();

	if (texture.surface != nullptr) {
		SDL_FreeSurface(texture.surface);
	}
	if (texture.sdlTexture != nullptr) {
		SDL_DestroyTexture(texture.sdlTexture);
	}

}

GameObject* Game::getGameObject(std::string name)
{
	std::optional<GameObject*> foundGameObject{};

	Scene& currentScene = SceneManager::instance().currentScene();

	for (auto& layer : currentScene.gameObjects()) {

		for (auto& gameObject : layer) {

			if (gameObject->name() == name) {

				foundGameObject = gameObject.get();
				break;
			}
		}
	}

	assert(foundGameObject.has_value() && "GameObject wasnt found!");

	return foundGameObject.value();
}

std::optional<SDL_Point> Game::_determineScreenResolution()
{

	//GameConfig::instance().targetScreenResolution()

	int numVideDisplays = SDL_GetNumVideoDisplays();
	if (numVideDisplays < 1) {
		assert(true && "SDL Video Display Detect failed!");
	}

	//We will default to the first display found
	SDL_DisplayMode displayMode{};
	int displayModes = SDL_GetNumDisplayModes(0);
	bool targetScreenResolutionFound{};
	bool fallbackScreenResolutionFound{};
	for (int x = 0; x < displayModes; x++) {
		SDL_GetDisplayMode(0, x, &displayMode);

		//Do we have the target screen resolution?
		if (displayMode.w == GameConfig::instance().targetScreenResolution().x &&
			displayMode.h == GameConfig::instance().targetScreenResolution().y) {
			targetScreenResolutionFound = true;
			break;
		}

		//Do we have the fallback screen resolution?
		if (displayMode.w == GameConfig::instance().fallbackScreenResolution().x &&
			displayMode.h == GameConfig::instance().fallbackScreenResolution().y) {
			fallbackScreenResolutionFound = true;
		}

	}

	std::optional<SDL_Point> screenResolution{};
	if (targetScreenResolutionFound == true) {
		screenResolution = GameConfig::instance().targetScreenResolution();
	}
	else if(fallbackScreenResolutionFound == true){
		screenResolution = GameConfig::instance().fallbackScreenResolution();
	}

	return screenResolution;

}