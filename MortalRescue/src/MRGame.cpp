#include "MRGame.h"

#include "GameConstants.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "LevelManager.h"
#include "MRContextManager.h"
#include "Camera.h"
#include "Clock.h"
#include "ContactFilter.h"
#include "ContactListener.h"
#include "components/DroneBrainComponent.h"
#include "IMGui/IMGuiUtil.h"
#include <fstream>

#include "EnumMaps.h"
#include "ColorMap.h"

#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

using namespace std::chrono_literals;


/*
Initialize Game
*/
bool MRGame::init(std::shared_ptr<ContactListener> contactListener, std::shared_ptr<ContactFilter> contactFilter,
	std::shared_ptr<ComponentFactory> componentFactory, std::shared_ptr<ActionFactory> actionFactory, std::shared_ptr<ParticleEffectsFactory> particleEffectsFactory,
	std::shared_ptr<CutSceneFactory> cutSceneFactory, std::shared_ptr<IMGuiFactory> iMGuiFactory, std::shared_ptr<TriggerFactory> triggerFactory, std::shared_ptr<ContextManager> contextManager)
{

	Game::init(contactListener, contactFilter, componentFactory, actionFactory, particleEffectsFactory, cutSceneFactory, iMGuiFactory, triggerFactory, contextManager);

	m_gameState = GameState::PLAY;

	//Add Game Specific constants
	_addGameCollisionTags();
	_addGameComponentTypes();
	_addGameColors();
	_addGameTraits();
	_addStatusItemTypes();


	//Init font library
	TTF_Init();

	//Initialize the texture manager
	m_renderer->init(m_window);

	//Initialize IMGUI
	ImGui::MobyDickInit(this);

	//Display basic loading message
	_displayLoadingMsg();

	TextureManager::instance().init();
	TextureManager::instance().load("textureAtlasAssets");

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
	GameObjectManager::instance().load("gameObjectDefinitions/guiObjects");
	GameObjectManager::instance().load("gameObjectDefinitions/hudObjects");
	GameObjectManager::instance().load("gameObjectDefinitions/titleScreenObjects");

	_displayLoadingMsg();

	//Load a first scene
	Scene& scene = SceneManager::instance().pushScene("SCENE_TITLE_SCREEN");

	//Load the current saved level
	//GameSaveFileData gameSaveFileData{};
	//contextManager->loadGame(&gameSaveFileData);
	//std::stringstream level{};
	//level << gameSaveFileData.level;
	//scene.loadLevel(level.str());

	//Initialize the sound manager
	SoundManager::instance().initSound();
	SoundManager::instance().playMusic("music_ambience_1", -1);

	//Initialize the clock object
	Clock::instance().init();

	//auto object = scene.addGameObject("GLOBAL_FRAME", GameLayer::BACKGROUND_1, 0, 0);
	//object->postInit();

	//auto object = scene.addGameObject("METEOR_EMITTER", GameLayer::BACKGROUND_1, 3, 7);
	//object->postInit();

	
	

	return true;
}


void MRGame::_addGameCollisionTags()
{

	//Add all game specific enum traslations needed
	EnumMap::instance().addEnumItem("ContactTag::BUTTON", ContactTag::BUTTON);
	EnumMap::instance().addEnumItem("ContactTag::DEFLECT_EFFECT", ContactTag::DEFLECT_EFFECT);
	EnumMap::instance().addEnumItem("ContactTag::DRONE_BRAIN", ContactTag::DRONE_BRAIN);
	EnumMap::instance().addEnumItem("ContactTag::DRONE_SHIELD", ContactTag::DRONE_SHIELD);
	EnumMap::instance().addEnumItem("ContactTag::DRONE_WEAPON", ContactTag::DRONE_WEAPON);
	EnumMap::instance().addEnumItem("ContactTag::DRONE_FRAME", ContactTag::DRONE_FRAME);
	EnumMap::instance().addEnumItem("ContactTag::ENEMY_BULLET", ContactTag::ENEMY_BULLET);
	EnumMap::instance().addEnumItem("ContactTag::FRIENDLY_BULLET", ContactTag::FRIENDLY_BULLET);
	EnumMap::instance().addEnumItem("ContactTag::HEAVY_PARTICLE", ContactTag::HEAVY_PARTICLE);
	EnumMap::instance().addEnumItem("ContactTag::LIGHT_PARTICLE", ContactTag::LIGHT_PARTICLE);
	EnumMap::instance().addEnumItem("ContactTag::PLAYER_COLLISION", ContactTag::PLAYER_COLLISION);;
	EnumMap::instance().addEnumItem("ContactTag::PLAYER_TOUCH", ContactTag::PLAYER_TOUCH);;
	EnumMap::instance().addEnumItem("ContactTag::PLAYER_HITBOX", ContactTag::PLAYER_HITBOX);;
	EnumMap::instance().addEnumItem("ContactTag::PLAYER_BULLET", ContactTag::PLAYER_BULLET);
	EnumMap::instance().addEnumItem("ContactTag::SHIELD_SCRAP", ContactTag::SHIELD_SCRAP);
	EnumMap::instance().addEnumItem("ContactTag::SURVIVOR", ContactTag::SURVIVOR);
	EnumMap::instance().addEnumItem("ContactTag::MEDKIT", ContactTag::MEDKIT);
	EnumMap::instance().addEnumItem("ContactTag::WALL", ContactTag::WALL);
	EnumMap::instance().addEnumItem("ContactTag::WEAPON_PICKUP", ContactTag::WEAPON_PICKUP);
	EnumMap::instance().addEnumItem("ContactTag::ESCAPE_STAIRS", ContactTag::ESCAPE_STAIRS);
	EnumMap::instance().addEnumItem("ContactTag::WALL_PIECE", ContactTag::WALL_PIECE);
	EnumMap::instance().addEnumItem("ContactTag::ENEMY_TURRET", ContactTag::ENEMY_TURRET);
	EnumMap::instance().addEnumItem("ContactTag::HEART_PICKUP", ContactTag::HEART_PICKUP);
	EnumMap::instance().addEnumItem("ContactTag::CONTAINER_INSIDE", ContactTag::CONTAINER_INSIDE);
	EnumMap::instance().addEnumItem("ContactTag::PLAYER_PULSE_BULLET", ContactTag::PLAYER_PULSE_BULLET);
	EnumMap::instance().addEnumItem("ContactTag::PLAYER_PULSE_EXPLOSION", ContactTag::PLAYER_PULSE_EXPLOSION);



}

void MRGame::_addGameComponentTypes()
{
	EnumMap::instance().addEnumItem("BRAIN_DRONE_COMPONENT", ComponentTypes::BRAIN_DRONE_COMPONENT);
	EnumMap::instance().addEnumItem("WEAPON_PISTOL_COMPONENT", ComponentTypes::WEAPON_PISTOL_COMPONENT);
	EnumMap::instance().addEnumItem("TURRET_BRAIN_COMPONENT", ComponentTypes::TURRET_BRAIN_COMPONENT);
	EnumMap::instance().addEnumItem("SURVIVOR_BRAIN_COMPONENT", ComponentTypes::SURVIVOR_BRAIN_COMPONENT);
	EnumMap::instance().addEnumItem("GINA_PLAYER_CONTROL_COMPONENT", ComponentTypes::GINA_PLAYER_CONTROL_COMPONENT);
	EnumMap::instance().addEnumItem("GINA_VITALITY_COMPONENT", ComponentTypes::GINA_VITALITY_COMPONENT);

}

void MRGame::_addStatusItemTypes()
{
	EnumMap::instance().addEnumItem("StatusItemId::PLAYERS_HEART_COUNT", StatusItemId::PLAYERS_HEART_COUNT);
	EnumMap::instance().addEnumItem("StatusItemId::PLAYER_WEAPON_LEVEL", StatusItemId::PLAYER_WEAPON_LEVEL);
	EnumMap::instance().addEnumItem("StatusItemId::SCRAP_COUNT", StatusItemId::SCRAP_COUNT);
	EnumMap::instance().addEnumItem("StatusItemId::PLAYER_WEAPON_ACCRUAL", StatusItemId::PLAYER_WEAPON_ACCRUAL);
	EnumMap::instance().addEnumItem("StatusItemId::SURVIVORS", StatusItemId::SURVIVORS);
	EnumMap::instance().addEnumItem("StatusItemId::PLAYER_WEAPON_CHARGED_PERCENT", StatusItemId::PLAYER_WEAPON_CHARGED_PERCENT);
	EnumMap::instance().addEnumItem("StatusItemId::PLAYER_WEAPON_IS_CHARGED", StatusItemId::PLAYER_WEAPON_IS_CHARGED);
}

void MRGame::_addGameTraits()
{
	

}

void MRGame::_addGameColors()
{

	ColorMap::instance().addColor("Colors::NOCOLOR", Colors::NOCOLOR);

	//greens
	ColorMap::instance().addColor("Colors::FOREST", Colors::FOREST);
	ColorMap::instance().addColor("Colors::EMERALD", Colors::EMERALD);
	ColorMap::instance().addColor("Colors::PARAKEET", Colors::PARAKEET);

	//reds
	ColorMap::instance().addColor("Colors::WINE", Colors::WINE);

	//yellows
	ColorMap::instance().addColor("Colors::GOLD", Colors::GOLD);

	//blues
	ColorMap::instance().addColor("Colors::CYAN", Colors::CYAN);
	ColorMap::instance().addColor("Colors::MARINE", Colors::MARINE);
	ColorMap::instance().addColor("Colors::SAPPHIRE", Colors::SAPPHIRE);

	//Dark
	ColorMap::instance().addColor("Colors::CHARCOAL", Colors::CHARCOAL);
	ColorMap::instance().addColor("Colors::MIRKWOOD", Colors::MIRKWOOD);



}


