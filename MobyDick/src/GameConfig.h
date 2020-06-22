#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <SDL2/SDL.h>
#include <box2d/box2d.h>

#include <string>

class GameConfig
{

public:
	static GameConfig& instance();
	bool init(std::string configFile);

	//Accessor functions
	float scaleFactor() {
		return m_scaleFactor;
	}
	float mouseSensitivity() {
		return m_mouseSensitivity;
	}
	SDL_Point debugPanelLocation() {
		return m_debugPanelLocation;
	}
	int debugPanelFontSize() {
		return m_debugPanelFontSize;
	}
	int soundChannels() {
		return m_soundChannels;
	}
	bool debugPanel() {
		return m_debugPanel;
	}
	std::string gameTitle() {
		return m_gameTitle;
	}
	int gameLoopStep() {
		return m_gameLoopStep;
	}
	b2Vec2 gravity() {
		return m_gravity;
	}
	float timeStep() {
		return m_timeStep;
	}
	int velocityIterations() {
		return m_velocityIterations;
	}
	int positionIterations() {
		return m_positionIterations;
	}
	bool b2DebugDrawMode() {
		return m_b2DebugDrawMode;
	}
	int windowWidth() {
		return m_windowWidth;
	}
	int windowHeight() {
		return m_windowHeight;
	}
	bool windowFullscreen() {
		return m_windowFullscreen;
	}
	float dynamicTextRefreshDelay() {
		return m_dynamicTextRefreshDelay;
	}

private:
	GameConfig();
	~GameConfig();

	float
		m_scaleFactor,
		m_mouseSensitivity,
		m_timeStep,
		m_dynamicTextRefreshDelay;
	bool
		m_windowFullscreen,
		m_debugPanel,
		m_b2DebugDrawMode;
	SDL_Point 
		m_debugPanelLocation;
	int
		m_windowWidth,
		m_windowHeight,
		m_debugPanelFontSize,
		m_soundChannels,
		m_gameLoopStep,
		m_velocityIterations,
		m_positionIterations;
	std::string 
		m_gameTitle;
	b2Vec2 
		m_gravity;


};

#endif