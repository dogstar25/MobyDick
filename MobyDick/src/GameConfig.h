#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <SDL2/SDL.h>

#pragma warning(push,0)
#include <box2d/box2d.h>
#pragma warning(pop)


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
		m_mouseSensitivity,
		m_dynamicTextRefreshDelay;
	bool
		m_windowFullscreen,
		m_debugPanel;
	SDL_Point 
		m_debugPanelLocation;
	int
		m_windowWidth,
		m_windowHeight,
		m_debugPanelFontSize,
		m_soundChannels,
		m_gameLoopStep;
	std::string 
		m_gameTitle;
	float m_scaleFactor{};


};

#endif