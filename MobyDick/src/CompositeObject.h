#ifndef COMPOSITE_OBJECT_H
#define COMPOSITE_OBJECT_H

#include <vector> 
#include <chrono>
#include <string> 

#include "WorldObject.h"
#include "GameObjectDefinition.h"


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
	void _buildPiece(CompositeLegendItem, int, int);
	void _updatePieces();
	void _buildComposite();
	void _updatePieceState(GameObjectPiece& piece);
	void _updatePiecePosition(GameObjectPiece& piece);
	void _levelUp(GameObjectPiece& piece);

	//Collection of all of the gameObjects that make up this composite object
	std::vector<GameObjectPiece>m_pieces;

};

#endif