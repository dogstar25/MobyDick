#pragma once
#include "WorldObject.h"
#include <vector> 
#include <SDL.h>
#include <map>
#include <chrono>
#include <string>

class GameObject;

using namespace std::chrono;
using namespace std;

class GameObjectPiece
{
	bool isDestroyed;
	bool shouldLevelUp;
	short level;
	float levelUpSpeed;
	shared_ptr<GameObject>gameObject;
	steady_clock::time_point time_snapshot;


};



class CompositeObject :
	public WorldObject
{

public:

	//Blueprint colors - BubbleGum16 Aseprite Pallette
	SDL_Color full = { 127,6,34,255 };
	SDL_Color angleBotRight = { 214,36,17,255 };
	SDL_Color angleBotLeft = { 255,132,38,255 };
	SDL_Color angleTopRight = { 255,209,0,255 };
	SDL_Color angleTopLeft = { 255,128,164,255 };

	CompositeObject();
	CompositeObject(string, float, float, float);
	~CompositeObject();

	void update();
	void render();
	void buildComposite();
	void updatePieces();

	//Collection of all of the gameObjects that make up this composite object
	vector<GameObjectPiece>pieces;

	Texture* blueprint;

};

