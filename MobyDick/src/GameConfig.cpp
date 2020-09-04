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
	std::ifstream ifs("assets/" + configFile + ".json");
	ifs >> root;

	m_gameTitle = root["gameTitle"].asString();
	m_gameLoopStep = root["gameLoopStep"].asInt();
	m_gravity.Set(root["physics"]["gravity"]["x"].asFloat(),
		root["physics"]["gravity"]["y"].asFloat());
	m_timeStep = root["physics"]["timeStep"].asFloat();
	m_velocityIterations = root["physics"]["velocityIterations"].asInt();
	m_positionIterations = root["physics"]["positionIterations"].asInt();
	m_b2DebugDrawMode = root["physics"]["b2DebugDrawMode"].asBool();

	m_scaleFactor = root["physics"]["box2dScale"].asFloat();
	m_mouseSensitivity = root["mouseSensitivity"].asFloat();
	m_dynamicTextRefreshDelay = root["dynamicTextRefreshDelay"].asFloat();
	m_debugPanel = root["debugPanel"]["show"].asBool();
	m_debugPanelLocation.x = root["debugPanel"]["xPos"].asInt();
	m_debugPanelLocation.y = root["debugPanel"]["yPos"].asInt();
	m_debugPanelFontSize = root["debugPanel"]["fontSize"].asInt();
	m_soundChannels = root["sound"]["numberOfChannels"].asInt();

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
