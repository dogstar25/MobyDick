#ifndef COMPOSITE_COMPONENT_H
#define COMPOSITE_COMPONENT_H

#include <vector>
#include <string>
#include <chrono>

#include <SDL2/SDL.h>
#include <box2d/box2d.h>

#include "Component.h"
#include "../GameObject.h"
#include "../Scene.h"

struct GameObjectPiece
{

	b2Vec2 parentPositionOffset = { 0,0 };
	std::shared_ptr<GameObject> pieceObject;

};

struct CompositeLegendItem
{
	SDL_Color color = {0,0,0,0};
	std::string gameObjectId;
};

struct CompositeBlueprint
{
	std::string textureId;
	std::vector<CompositeLegendItem> legend;
};

class CompositeComponent : public Component
{
public:
	CompositeComponent();
	CompositeComponent(Json::Value componentJSON, Scene* parentScene);
	~CompositeComponent();

	void update() override;
	void render();
	void weldOnPieces();
	void postInit();
	bool physicsWeldPiecesOn() { return m_physicsWeldPiecesOn; }
	void setDetachAllPieces() { m_detachAllPieces = true; }
	std::vector<GameObjectPiece>& pieces() { return m_pieces; };
	

private:
	bool m_physicsWeldPiecesOn;
	
	CompositeBlueprint m_blueprint;

	void _buildPiece(CompositeLegendItem, int, int, Scene* parentScene);
	void _updatePieces();
	void _buildComposite(Scene* parentScene);
	void _updatePieceState(GameObjectPiece& piece);
	void _updatePiecePosition(GameObjectPiece& piece);
	void _removeFromWorldPass();
	void _detachAllPieces();
	

	std::vector<GameObjectPiece>m_pieces;
	bool m_detachAllPieces{false};

};

#endif

