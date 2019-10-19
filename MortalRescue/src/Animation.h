#pragma once
#include <vector> 
#include <SDL.h>
#include <map>
#include <chrono>
#include <string>

class GameObject;
class Game;

extern Game* game;

using namespace std::chrono;
using namespace std;

class Animation
{
public:
	string id;
	float speed;
	int currentAnimFrame;
	int frameCount;
	steady_clock::time_point time_snapshot;

	//a rectangle pointing to the animation textture of the animation frame to be displayed
	SDL_Rect currentTextureAnimationSrcRect;

	//Array of all x,y coordinates of the top left corner of each animation frame in the texture
	vector<SDL_Point> animationFramePositions;
	SDL_Texture* texture;

	void animate(GameObject*);

private:
};

