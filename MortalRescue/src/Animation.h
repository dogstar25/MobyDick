#pragma once
#include <vector> 
#include <SDL.h>
#include <map>
#include <chrono>
#include <string>


class GameObject;
class GameObjectDefinition;


using namespace std::chrono;
using namespace std;

class Animation
{

private:
	string id;
	float speed;
	
	int frameCount;
	b2Vec2 frameSize;
	steady_clock::time_point time_snapshot;

	//a rectangle pointing to the animation textture of the animation frame to be displayed
	SDL_Rect* currentTextureAnimationSrcRect;

	//Array of all x,y coordinates of the top left corner of each animation frame in the texture
	vector<SDL_Point> animationFramePositions;
	SDL_Texture* texture;

public:

	int currentAnimFrame;

	Animation(GameObjectDefinition* ,string ,string ,int ,float );
	~Animation();

	void animate();
	SDL_Rect* getRenderTextureRect();
	SDL_Texture* getRenderTexture();

};

