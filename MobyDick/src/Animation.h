#ifndef ANIMATION_H
#define ANIMATION_H

#include <chrono>
#include <string>
#include <vector> 
#include <SDL2/SDL.h>
#include <Box2D/Box2D.h>
#include <json/json.h>

class GameObjectDefinition;

class Animation
{

public:

	Animation();
	Animation(Json::Value animationDetailsJSON, Json::Value transformComponentJSON);
	Animation(GameObjectDefinition* gameObjectDefinition, std::string animationId, std::string textureId,
		int totalFrames, float speed);
	~Animation();

	void animate();

	//Accessor Functions
	std::shared_ptr<SDL_Rect> getCurrentTextureAnimationSrcRect() {
		return m_currentTextureAnimationSrcRect;
	}
	SDL_Texture* getTexture() {
		return m_texture;
	}
	int getCurrentAnimFrame() {
		return m_currentAnimFrame;
	}


private:
	;
	float 
		m_speed;
	int 
		m_state,
		m_frameCount,
		m_currentAnimFrame;

	b2Vec2 
		m_frameSize;

	std::chrono::steady_clock::time_point m_timeSnapshot;

	//a rectangle pointing to the animation textture of the animation frame to be displayed
	std::shared_ptr<SDL_Rect> m_currentTextureAnimationSrcRect;

	//Array of all x,y coordinates of the top left corner of each animation frame in the texture
	std::vector<SDL_FPoint> m_animationFramePositions;

	SDL_Texture* m_texture;

};

#endif