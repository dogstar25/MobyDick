#ifndef ANIMATION_H
#define ANIMATION_H

#include <chrono>
#include <string>
#include <vector> 
#include <SDL2/SDL.h>
#include <Box2D/Box2D.h>

class GameObjectDefinition;

class Animation
{

public:

	Animation(GameObjectDefinition* gameObjectDefinition, std::string animationId, std::string textureId,
		int totalFrames, float speed);
	~Animation();

	void animate();

	//Accessor Functions
	SDL_Rect* getCurrentTextureAnimationSrcRect() {
		return m_currentTextureAnimationSrcRect;
	}
	SDL_Texture* getTexture() {
		return m_texture;
	}
	int getCurrentAnimFrame() {
		return m_currentAnimFrame;
	}


private:
	std::string m_id;
	float m_speed;
	int m_frameCount;
	int m_currentAnimFrame;
	b2Vec2 m_frameSize;
	std::chrono::steady_clock::time_point m_timeSnapshot;

	//a rectangle pointing to the animation textture of the animation frame to be displayed
	SDL_Rect* m_currentTextureAnimationSrcRect;

	//Array of all x,y coordinates of the top left corner of each animation frame in the texture
	std::vector<SDL_Point> m_animationFramePositions;

	SDL_Texture* m_texture;

};

#endif