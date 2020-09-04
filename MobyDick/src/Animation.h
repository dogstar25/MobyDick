#ifndef ANIMATION_H
#define ANIMATION_H

#include <chrono>
#include <string>
#include <vector> 
#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include <json/json.h>
#include "Globals.h"

class GameObjectDefinition;

class Animation
{

public:

	Animation();
	Animation(Json::Value animationDetailsJSON, Json::Value transformComponentJSON);
	Animation(GameObjectDefinition* gameObjectDefinition, std::string animationId, std::string textureId,
		int totalFrames, float speed);
	~Animation();

	int animate();

	//Accessor Functions
	std::shared_ptr<SDL_Rect> getCurrentTextureAnimationSrcRect() { return m_currentTextureAnimationSrcRect; }
	SDL_Texture* getTexture() { return m_texture; }
	int getCurrentAnimFrame() { return m_currentAnimFrame; }


private:
	float   m_speed{ 0 };
	int     m_state{ ANIMATION_IDLE };
	int     m_frameCount{ 0 };
	int     m_currentAnimFrame{ 0 };

	b2Vec2 	m_frameSize{ 0,0 };

	std::chrono::steady_clock::time_point m_timeSnapshot{ std::chrono::duration<int>(0) };

	//a rectangle pointing to the animation textture of the animation frame to be displayed
	std::shared_ptr<SDL_Rect> m_currentTextureAnimationSrcRect;

	//Array of all x,y coordinates of the top left corner of each animation frame in the texture
	std::vector<SDL_FPoint> m_animationFramePositions;

	SDL_Texture* m_texture;

};

#endif