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

struct GameObjectPiece
{

	bool isDestroyed;
	b2Vec2 parentPositionOffset;
	short currentlevel;
	steady_clock::time_point time_snapshot;
	WorldObject* pieceObject;
	
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
	void buildPiece(CompositeLegendItem, int,int);
	void updatePieces();

	void updatePieceState(GameObjectPiece& piece);
	void updatePiecePosition(GameObjectPiece& piece);
	void levelUp(GameObjectPiece& piece);

	//Collection of all of the gameObjects that make up this composite object
	vector<GameObjectPiece>pieces;

};

