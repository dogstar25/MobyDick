#pragma once

#include <map>
#include <chrono>
#include <string>
#include <vector> 
#include <SDL.h>
#include <Box2D/Box2D.h>
#include "Game.h"

class Animation
{

private:
	std::string id;
	float speed;
	
	int frameCount;
	b2Vec2 frameSize;
	std::chrono::steady_clock::time_point time_snapshot;

	//a rectangle pointing to the animation textture of the animation frame to be displayed
	SDL_Rect* currentTextureAnimationSrcRect;

	//Array of all x,y coordinates of the top left corner of each animation frame in the texture
	std::vector<SDL_Point> animationFramePositions;
	SDL_Texture* texture;

public:

	int currentAnimFrame;

	Animation(GameObjectDefinition* gameObjectDefinition, std::string animationId, std::string textureId,
		int totalFrames, float speed);
	~Animation();

	void animate();
	SDL_Rect* getRenderTextureRect();
	SDL_Texture* getRenderTexture();

};

