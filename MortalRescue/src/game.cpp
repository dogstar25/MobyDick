#include "Game.h"

#include "actions/ActionMaps.h"
#include "GameConstants.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "LevelManager.h"
#include "Camera.h"
#include "Clock.h"
#include "ContactFilter.h"
#include "ContactListener.h"
#include "components/DroneBrainComponent.h"
#include "particleEffects/ParticleEffectsMap.h"
#include "particleEffects/GameParticleEffects.h"

#include "EnumMaps.h"

#include "actions/MyAction.h"



using namespace std::chrono_literals;

Game& Game::instance()
{
	static Game singletonInstance;
	return singletonInstance;
}

Game::~Game()
{

	printf("cleaning game\n");

	//Delete SDL stuff
	SDL_DestroyWindow(m_window);
	SDL_Quit();
	TTF_Quit();

}

Game::Game()
{

	m_window = nullptr;
	

}

/*
Initialize Game
*/
bool Game::init()
{

	m_gameState = GameState::PLAY;

	//Add Game Specific stuff
	_addGameCollisionTags();
	_addGameActions();
	_addGameParticleEffects();

	//Get all of the configuration values
	GameConfig::instance().init("gameConfig");
	
	//Initialize world
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{

		//Init font library
		TTF_Init();

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
		m_window = SDL_CreateWindow(GameConfig::instance().gameTitle().c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			GameConfig::instance().windowWidth(),
			GameConfig::instance().windowHeight(),
			windowFlags);

		//Initialize the texture manager
		Renderer::instance().init(m_window);

		//Display basic loading message
		_displayLoadingMsg();

		//Initialize the texture manager
		TextureManager::instance().init();
		TextureManager::instance().load("textureAssets");

		//Initialize the SceneManager
		SceneManager::instance().init();
		SceneManager::instance().load("gameScenes");

		_displayLoadingMsg();

		//Initialize the Game Object Manager
		GameObjectManager::instance().init();
		GameObjectManager::instance().load("gameObjectDefinitions/commonObjects");
		GameObjectManager::instance().load("gameObjectDefinitions/wallObjects");
		GameObjectManager::instance().load("gameObjectDefinitions/particleObjects");
		GameObjectManager::instance().load("gameObjectDefinitions/compositeObjects");

		//Load a first scene
		Scene& scene = SceneManager::instance().pushScene("SCENE_PLAY");
		scene.loadLevel("level1");

		_displayLoadingMsg();

		//Initialize the sound manager
		SoundManager::instance().initSound();
		SoundManager::instance().playMusic("music_ambience_1", -1);

		//Initialize the clock object
		Clock::instance().init();

		scene.addGameObject("FPS_VALUE", LAYER_MENU, 1, 1);

	}

	//test
	auto particleXEmitterObject = SceneManager::instance().addGameObject("PARTICLE_EMITTER_SPARK", LAYER_MAIN, 9, 9);

	return true;
}


/*
Main Play Loop
*/
void Game::play()
{

	while (m_gameState != GameState::QUIT)
	{

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

			//Apply the mouse control mode based on what the new "current" scene wants
			//SceneManager::instance().scenes().back().applyCurrentControlMode();
		}

		SceneManager::instance().run();
	}

}

void Game::_addGameActions()
{

	ActionMaps::instance().addAction("MyAction", std::make_shared<MyAction>());

}

void Game::_addGameParticleEffects()
{

	ParticleEffectsMap::instance().addParticleEffect("ricochet", ParticleEffects::ricochet);
	ParticleEffectsMap::instance().addParticleEffect("ricochetX", ParticleEffects::ricochetX);
	ParticleEffectsMap::instance().addParticleEffect("deflect", ParticleEffects::deflect);
	ParticleEffectsMap::instance().addParticleEffect("scrap", ParticleEffects::scrap);
	ParticleEffectsMap::instance().addParticleEffect("spark", ParticleEffects::spark);

}

void Game::_addGameCollisionTags()
{

	//Add all game specific enum traslations needed
	EnumMap::instance().addEnumItem("CollisionTag::DEFLECT_EFFECT", CollisionTag::DEFLECT_EFFECT);
	EnumMap::instance().addEnumItem("CollisionTag::SMOKE_PARTICLE", CollisionTag::SMOKE_PARTICLE);
	EnumMap::instance().addEnumItem("CollisionTag::DRONE_BRAIN", CollisionTag::DRONE_BRAIN);
	EnumMap::instance().addEnumItem("CollisionTag::DRONE_SHIELD", CollisionTag::DRONE_SHIELD);
	EnumMap::instance().addEnumItem("CollisionTag::DRONE_WEAPON", CollisionTag::DRONE_WEAPON);
	EnumMap::instance().addEnumItem("CollisionTag::DRONE_FRAME", CollisionTag::DRONE_FRAME);
	EnumMap::instance().addEnumItem("CollisionTag::ENEMY_BULLET", CollisionTag::ENEMY_BULLET);
	EnumMap::instance().addEnumItem("CollisionTag::FRIENDLY_BULLET", CollisionTag::FRIENDLY_BULLET);
	EnumMap::instance().addEnumItem("CollisionTag::HEAVY_PARTICLE", CollisionTag::HEAVY_PARTICLE);
	EnumMap::instance().addEnumItem("CollisionTag::LIGHT_PARTICLE", CollisionTag::LIGHT_PARTICLE);
	EnumMap::instance().addEnumItem("CollisionTag::PLAYER", CollisionTag::PLAYER);;
	EnumMap::instance().addEnumItem("CollisionTag::PLAYER_BULLET", CollisionTag::PLAYER_BULLET);
	EnumMap::instance().addEnumItem("CollisionTag::SHIELD_SCRAP", CollisionTag::SHIELD_SCRAP);
	EnumMap::instance().addEnumItem("CollisionTag::SURVIVOR", CollisionTag::SURVIVOR);
	EnumMap::instance().addEnumItem("CollisionTag::MEDKIT", CollisionTag::MEDKIT);
	EnumMap::instance().addEnumItem("CollisionTag::WALL", CollisionTag::WALL);
	EnumMap::instance().addEnumItem("CollisionTag::WEAPON_PICKUP", CollisionTag::WEAPON_PICKUP);


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