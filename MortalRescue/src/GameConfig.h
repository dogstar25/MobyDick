#pragma once
#include <SDL.h>
#include <Box2D/Box2D.h>

#include <string>

class GameConfig
{
public:
	static GameConfig& instance();
	bool init(std::string configFile);

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
	SDL_Rect defaultCameraFrame() {
		return m_DefaultCameraFrame;
	}

private:
	GameConfig();
	~GameConfig();

	float
		m_scaleFactor,
		m_mouseSensitivity,
		m_timeStep;
	bool
		m_debugPanel,
		m_b2DebugDrawMode;
	SDL_Point 
		m_debugPanelLocation;
	SDL_Rect
		m_DefaultCameraFrame;
	int
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

