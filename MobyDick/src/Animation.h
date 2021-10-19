#ifndef ANIMATION_H
#define ANIMATION_H

#include <chrono>
#include <string>
#include <vector> 
#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include <jsoncpp/include/json/json.h>
#include "Globals.h"
#include "BaseConstants.h"
#include "Timer.h"

#include "texture.h"

class GameObjectDefinition;

class Animation
{

public:

	Animation() = default;
	Animation(Json::Value animationDetailsJSON, b2Vec2 frameSize);

	int animate();

	//Accessor Functions
	std::shared_ptr<SDL_Rect> getCurrentTextureAnimationSrcRect() { return m_currentTextureAnimationSrcRect; }
	std::shared_ptr<Texture> getTexture() { return m_texture; }
	int getCurrentAnimFrame() { return m_currentAnimFrame; }


private:
	float   m_speed{ 0 };
	int     m_state{ ANIMATION_IDLE };
	int     m_frameCount{ 0 };
	int     m_currentAnimFrame{ 0 };

	b2Vec2 	m_frameSize{ 0,0 };

	//a rectangle pointing to the animation textture of the animation frame to be displayed
	std::shared_ptr<SDL_Rect> m_currentTextureAnimationSrcRect;

	//Array of all x,y coordinates of the top left corner of each animation frame in the texture
	std::vector<SDL_FPoint> m_animationFramePositions;
	std::shared_ptr<Texture> m_texture{};
	Timer m_timer;

};

#endif