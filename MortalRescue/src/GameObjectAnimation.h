#pragma once
#include <vector> 
#include <SDL.h>
using namespace std;

class GameObjectAnimation
{
public:
	GameObjectAnimation();
	~GameObjectAnimation();

private:
	vector<SDL_Texture*> frames;
};

