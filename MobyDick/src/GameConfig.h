#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <SDL2/SDL.h>

#include <box2d/box2d.h>

#include <string>

#include "BaseConstants.h"

class GameConfig
{

public:
	static GameConfig& instance();
	bool init(std::string configFile);

	//Accessor functions
	float scaleFactor() {
		return m_scaleFactor;
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
	std::string gameTitle() {
		return m_gameTitle;
	}
	float gameLoopStep() {
		return m_gameLoopStep;
	}
	bool windowFullscreen() {
		return m_windowFullscreen;
	}
	float dynamicTextRefreshDelay() {
		return m_dynamicTextRefreshDelay;
	}
	RendererType rendererType() {
		return m_rendererType;
	}
	bool openGLBatching() {
		return m_openGLBatching;
	}
	SDL_Point targetScreenResolution() {
		return m_targetScreenResolution;
	}
	SDL_Point fallbackScreenResolution() {
		return m_fallbackScreenResolution;
	}
	SDL_Point defaultTileSize() {
		return m_defaultTileSize;
	}
	bool debugGrid() {
		return m_debugGrid;
	}

private:
	GameConfig();
	~GameConfig();

	RendererType m_rendererType;

	float m_dynamicTextRefreshDelay{};
	float m_gameLoopStep{};
	bool m_windowFullscreen{};
	bool m_openGLBatching{};
	bool m_debugGrid{};
	SDL_Point m_debugPanelLocation{};
	int	m_debugPanelFontSize{};
	int m_soundChannels{};

	std::string m_gameTitle;
	float m_scaleFactor{};
	SDL_Point m_screenResolution{};
	SDL_Point m_targetScreenResolution{};
	SDL_Point m_fallbackScreenResolution{};
	SDL_Point m_defaultTileSize{};


};

#endif