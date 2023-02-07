#include "GameConfig.h"

#include "game.h"
#include "EnumMap.h"

#include <fstream>

extern std::unique_ptr<Game> game;

GameConfig::GameConfig()
{


}

GameConfig::~GameConfig()
{

}

bool GameConfig::init(std::string configFile)
{

	Json::Value root;
	std::ifstream ifs("assets/config/" + configFile + ".json");
	ifs >> root;

	m_gameTitle = root["gameTitle"].asString();
	m_gameLoopStep = root["gameLoopStep"].asFloat();
	m_dynamicTextRefreshDelay = root["dynamicTextRefreshDelay"].asFloat();
	m_debugPanelLocation.x = root["debugPanel"]["xPos"].asInt();
	m_debugPanelLocation.y = root["debugPanel"]["yPos"].asInt();
	m_debugPanelFontSize = root["debugPanel"]["fontSize"].asInt();
	m_soundChannels = root["sound"]["numberOfChannels"].asInt();
	m_scaleFactor = root["physics"]["box2dScale"].asFloat();
	m_rendererType = static_cast<RendererType>(game->enumMap()->toEnum(root["rendererType"].asString()));
	m_openGLBatching = root["openGLBatching"].asBool();

	m_debugGrid = root["debugGrid"].asBool();

	Json::Value windowJSON = root["window"];
	m_defaultTileSize = { windowJSON["defaultTileSize"]["width"].asInt(), windowJSON["defaultTileSize"]["height"].asInt() };
	m_targetScreenResolution = { windowJSON["targetScreenResolution"]["width"].asInt(), windowJSON["targetScreenResolution"]["height"].asInt() };
	m_fallbackScreenResolution = { windowJSON["fallbackScreenResolution"]["width"].asInt(), windowJSON["fallbackScreenResolution"]["height"].asInt() };

	m_windowFullscreen = root["window"]["fullscreen"].asBool();

	return true;
}

GameConfig& GameConfig::instance()
{

	static GameConfig singletonInstance;
	return singletonInstance;

}
