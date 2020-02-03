#pragma once
#include <string>
#include <map>
#include <SDL.h>
#include <Box2D/Box2D.h>
#include <cstddef>

#include "Animation.h"


using namespace std;

struct TextDetails {

	SDL_Color color;
	int size;
	bool isDynamic;
	string fontId;
	string value;

};

struct ChildObjectDetails
{
	string gameObjectId;
	short position;
	bool absolutePositioning;
	string gameObjectType;

};

struct CompositeLevel
{
	short levelNum;
	short strength;
	SDL_Color color;
};

struct CompositeLegendItem
{
	SDL_Color color;
	string gameObjectId;
	string gameObjectType;
};

/*
Texture file representing the maps of the composite object and the legend that has the info
Of how to decipher the pixels in the blueprint texture
*/
struct CompositeBlueprint
{
	string textureId;
	vector<CompositeLegendItem> legend;
};

struct CompositeDetails
{
	short maxlevel;
	float levelUpSpeed;
	vector<CompositeLevel> levels;
	CompositeBlueprint blueprint;
};


class GameObjectDefinition
{
public:

	GameObjectDefinition();
	~GameObjectDefinition();

	string
		id,
		type,
		description,
		physicsType,
		collisionShape,
		textureId,
		onClickAction;
	float
		xSize,
		ySize,
		speed,
		friction,
		restitution,
		density,
		linearDamping,
		collisionRadius,
		angularDamping,
		childPadding,
		lifetime,
		xRenderAdjustment, 
		yRenderAdjustment;
	bool
		isAnimated,
		isPhysicsObject,
		isPlayerObject,
		isTextObject,
		isParticle,
		isCompositeObject,
		isMouseSelectable,
		hasChildObjects,
		absolutePositioning,
		renderOutline,
		childPositionRelative;
	uint16
		collisionCategory;

	/*
	Complex definition details
	*/
	TextDetails textDetails;
	CompositeDetails compositeDetails;
	SDL_Color color; //The color to apply to the object
	map<string, Animation*> animations;
	vector<ChildObjectDetails> childObjectDefinitions;



};