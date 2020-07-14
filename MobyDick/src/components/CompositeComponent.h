#ifndef COMPOSITE_COMPONENT_H
#define COMPOSITE_COMPONENT_H

#include <vector>
#include <string>
#include <SDL2/SDL.h>

#include "Component.h"


struct CompositeLevel
{
	short levelNum;
	short strength;
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
	~CompositeComponent();

	void update(std::shared_ptr<GameObject>gameObject) override;


private:
	short maxlevel;
	float levelUpSpeed;
	//std::vector<CompositeLevel> levels;
	CompositeBlueprint blueprint;

	//std::vector<GameObjectPiece>m_pieces;

};

#endif

