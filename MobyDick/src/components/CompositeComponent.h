#ifndef COMPOSITE_COMPONENT_H
#define COMPOSITE_COMPONENT_H

#include <vector>
#include <string>
#include <chrono>

#include <SDL2/SDL.h>
#include <box2d/box2d.h>

#include "Component.h"
#include "../GameObject.h"

struct GameObjectPiece
{

	bool isDestroyed;
	b2Vec2 parentPositionOffset;
	short currentlevel;
	std::chrono::steady_clock::time_point time_snapshot;
	GameObject* pieceObject;

};


struct CompositeLevel
{
	short levelNum;
	short durability;
	SDL_Color color;
};

struct CompositeLegendItem
{
	SDL_Color color;
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
	CompositeComponent(Json::Value definitionJSON);
	~CompositeComponent();

	void update() override;
	void render();


private:
	short m_maxlevel;
	float m_levelUpSpeed;
	std::vector<CompositeLevel> m_levels;
	CompositeBlueprint m_blueprint;

	void _buildPiece(CompositeLegendItem, int, int);
	void _updatePieces();
	void _buildComposite();
	void _updatePieceState(GameObjectPiece& piece);
	void _updatePiecePosition(GameObjectPiece& piece);
	void _levelUp(GameObjectPiece& piece);
	b2Vec2 _matchParentRotation(SDL_FPoint childPosition, SDL_FPoint parentPosition, float parentAngle);

	std::vector<GameObjectPiece>m_pieces;

};

#endif

