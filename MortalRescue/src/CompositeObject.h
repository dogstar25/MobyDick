#pragma once
#include "WorldObject.h"
#include <vector> 
#include <SDL.h>
#include <map>
#include <chrono>
#include <string>

class GameObject;

struct GameObjectPiece
{

	bool isDestroyed;
	b2Vec2 parentPositionOffset;
	short currentlevel;
	std::chrono::steady_clock::time_point time_snapshot;
	WorldObject* pieceObject;
	
};


class CompositeObject :
	public WorldObject
{

public:

	CompositeObject();
	CompositeObject(std::string, float, float, float);
	~CompositeObject();

	void update();
	void render();
	

private:
	void buildPiece(CompositeLegendItem, int, int);
	void updatePieces();
	void buildComposite();
	void updatePieceState(GameObjectPiece& piece);
	void updatePiecePosition(GameObjectPiece& piece);
	void levelUp(GameObjectPiece& piece);

	//Collection of all of the gameObjects that make up this composite object
	vector<GameObjectPiece>m_pieces;

};

