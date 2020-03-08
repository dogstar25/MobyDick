#include "GameConfig.h"
#include <json/json.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>

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
	//ifs.close();

	m_gameTitle = root["gameTitle"].asString();
	m_gameLoopStep = root["gameLoopStep"].asInt();
	m_gravity.Set(root["physics"]["gravity"]["x"].asInt(), 
		root["physics"]["gravity"]["y"].asFloat());
	m_timeStep = root["physics"]["timeStep"].asFloat();
	m_velocityIterations = root["physics"]["velocityIterations"].asInt();
	m_positionIterations = root["physics"]["positionIterations"].asInt();
	m_b2DebugDrawMode = root["physics"]["b2DebugDrawMode"].asBool();

	m_scaleFactor = root["physics"]["box2dScale"].asFloat();
	m_mouseSensitivity = root["mouseSensitivity"].asFloat();
	m_debugPanel = root["debugPanel"]["show"].asBool();
	m_debugPanelLocation.x = root["debugPanel"]["xPos"].asInt();
	m_debugPanelLocation.y = root["debugPanel"]["yPos"].asInt();
	m_debugPanelFontSize = root["debugPanel"]["fontSize"].asInt();
	m_soundChannels = root["sound"]["numberOfChannels"].asInt();

	m_DefaultCameraFrame.x = 0;
	m_DefaultCameraFrame.y = 0;
	m_DefaultCameraFrame.w = root["camera"]["width"].asInt();
	m_DefaultCameraFrame.h = root["camera"]["height"].asInt();

	return true;
}

GameConfig& GameConfig::instance()
{

	static GameConfig singletonInstance;
	return singletonInstance;

}
