#include "GameConfig.h"

#include "game.h"

#include <fstream>

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
	m_mouseSensitivity = root["mouseSensitivity"].asFloat();
	m_dynamicTextRefreshDelay = root["dynamicTextRefreshDelay"].asFloat();
	m_debugPanel = root["debugPanel"]["show"].asBool();
	m_debugPanelLocation.x = root["debugPanel"]["xPos"].asInt();
	m_debugPanelLocation.y = root["debugPanel"]["yPos"].asInt();
	m_debugPanelFontSize = root["debugPanel"]["fontSize"].asInt();
	m_soundChannels = root["sound"]["numberOfChannels"].asInt();
	m_scaleFactor = root["physics"]["box2dScale"].asFloat();

	m_windowWidth = root["window"]["width"].asInt();
	m_windowHeight = root["window"]["height"].asInt();
	m_windowFullscreen = root["window"]["fullscreen"].asBool();

	//Global World sizes
	SDL_Rect bounds = { m_windowWidth, m_windowHeight };
	Game::instance().setWorldParams(bounds, 32, 32);

	return true;
}

GameConfig& GameConfig::instance()
{

	static GameConfig singletonInstance;
	return singletonInstance;

}
