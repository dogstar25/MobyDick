#include "MRGame.h"

#include "GameConstants.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "LevelManager.h"
#include "MRStatusManager.h"
#include "Camera.h"
#include "Clock.h"
#include "ContactFilter.h"
#include "ContactListener.h"
#include "components/DroneBrainComponent.h"
#include "IMGui/IMGuiUtil.h"

#include "EnumMaps.h"


#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"


using namespace std::chrono_literals;


MRGame::MRGame()
{

}

MRGame::~MRGame()
{

}

/*
Initialize Game
*/
bool MRGame::init(ContactListener* contactListener, ContactFilter* contactFilter, 
	ComponentFactory* componentFactory, ActionFactory* actionFactory, ParticleEffectsFactory* particleEffectsFactory, 
	CutSceneFactory* cutSceneFactory, IMGuiFactory* iMGuiFactory, StatusManager* statusManager)
{

	//Get all of the configuration values
	GameConfig::instance().init("gameConfig");

	Game::init(contactListener, contactFilter, componentFactory, actionFactory, particleEffectsFactory, cutSceneFactory, iMGuiFactory, statusManager);

	std::cout << "Mortal Rescue Begins\n";

	m_gameState = GameState::PLAY;

	//Add Game Specific stuff
	_addGameCollisionTags();
	_addGameComponentTypes();

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
		m_renderer->init(m_window);

		
		//Display basic loading message
		//_displayLoadingMsg();


		//Test for IMGUI
		ImGui::MobyDickInit(this);

		//////////////////////////////////////////////////////
		//ImGui::CreateContext();

		//ImGuiIO& io = ImGui::GetIO(); (void)io;

		//ImGui::StyleColorsDark();

		//auto gl_context = SDL_GL_GetCurrentContext();
		//const char* glsl_version = "#version 130";

		//// Setup Platform/Renderer backends
		//if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

		//	ImGui_ImplSDL2_InitForOpenGL(window(), gl_context);
		//	ImGui_ImplOpenGL3_Init(glsl_version);
		//}
		//else {
		//	ImGui_ImplSDL2_InitForSDLRenderer(window());
		//	ImGui_ImplSDLRenderer_Init(renderer()->sdlRenderer());
		//}

		///////////////////////////////////////////


		TextureManager::instance().init();
		TextureManager::instance().load("textureAtlasAssets");

		//Initialize the SceneManager
		SceneManager::instance().init();
		SceneManager::instance().load("gameScenes");


		//_displayLoadingMsg();

		//Initialize the Game Object Manager
		GameObjectManager::instance().init();
		GameObjectManager::instance().load("gameObjectDefinitions/commonObjects");
		GameObjectManager::instance().load("gameObjectDefinitions/wallObjects");
		GameObjectManager::instance().load("gameObjectDefinitions/particleObjects");
		GameObjectManager::instance().load("gameObjectDefinitions/compositeObjects");
		GameObjectManager::instance().load("gameObjectDefinitions/guiObjects");
		GameObjectManager::instance().load("gameObjectDefinitions/hudObjects");

		//Load a first scene
		Scene& scene = SceneManager::instance().pushScene("SCENE_PLAY");
		scene.loadLevel("level1");

		//scene.addHudItemValue("hudId", &MRStatusManager::level());

		//_displayLoadingMsg();

		//Initialize the sound manager
		//SoundManager::instance().initSound();
		//SoundManager::instance().playMusic("music_ambience_1", -1);

		//Initialize the clock object
		Clock::instance().init();

		
		
		//scene.addGameObject("WALL1_CORNER", LAYER_MENU, 5, 5);
		//scene.addGameObject("STUBB", LAYER_MENU, 1, 1);

		//scene.addGameObject("GINA_64", LAYER_MENU, 8, 8);
		
		//scene.addGameObject("HUD_STATUS", LAYER_MENU, 16, 1);
		//scene.addGameObject("HUD_STATUS_TOP", LAYER_MENU, 2, 2);
		//scene.addGameObject("HUD_TEST_LABEL", LAYER_MENU, 2, 2);

		scene.addGameObject("GLOW_TRIANGLE", LAYER_MENU, 3, 3);
		scene.addGameObject("PARTICLE_EMITTER_SPARK", LAYER_MENU, 13, 13);
		//scene.addGameObject("PARTICLE_EMITTER_SPARK", LAYER_MENU, 1, 1);
		scene.addGameObject("BOWMAN", LAYER_MENU, 3, 3);

		
		//{
		//	ImFont* m_font = io.Fonts->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 22.0f);
		//	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

		//	//io.Fonts->Build();

			//if (GameConfig::instance().rendererType() == RendererType::OPENGL) {
			//	ImGui_ImplOpenGL3_CreateFontsTexture();
			//}
			//else {
			//	ImGui_ImplSDLRenderer_CreateFontsTexture();
			//}

			//if (GameConfig::instance().rendererType() == RendererType::OPENGL) {
			//	ImGui_ImplOpenGL3_NewFrame();
			//	ImGui_ImplSDL2_NewFrame();
			//}
			//else {
			//	ImGui_ImplSDLRenderer_NewFrame();
			//	ImGui_ImplSDL2_NewFrame();
			//}
			//ImGui::NewFrame();

		//}


	}

	return true;
}


void MRGame::_addGameCollisionTags()
{

	//Add all game specific enum traslations needed
	EnumMap::instance().addEnumItem("CollisionTag::DEFLECT_EFFECT", CollisionTag::DEFLECT_EFFECT);
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

void MRGame::_addGameComponentTypes()
{
	EnumMap::instance().addEnumItem("BRAIN_DRONE_COMPONENT", ComponentTypes::BRAIN_DRONE_COMPONENT);
	EnumMap::instance().addEnumItem("WEAPON_PISTOL_COMPONENT", ComponentTypes::WEAPON_PISTOL_COMPONENT);
	EnumMap::instance().addEnumItem("TURRET_BRAIN_COMPONENT", ComponentTypes::TURRET_BRAIN_COMPONENT);
}


