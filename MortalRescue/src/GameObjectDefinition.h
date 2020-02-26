#pragma once
#include <string>
#include <map>
#include <SDL.h>
#include <Box2D/Box2D.h>

#include <cstddef>
#include <vector>
#include "Weapon.h"

class Animation;

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
	short locationSlot;
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

struct AnimationItem
{
	string state;
	string textureId;
	float speed;
	int frames;

};

struct AnimationDetails
{
	vector<AnimationItem> animations;
};

/*
ToDO:All of the members should be const because they cannot be changed. If they can, then they need
to move to the gameObject level
*/
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
		yRenderAdjustment,
		fireOffset;
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
	AnimationDetails animationDetails;
	SDL_Color color;

	vector<ChildObjectDetails> childObjectDefinitions; //TODO:turn this into ChildDetails

	map<int, shared_ptr<Weapon>> weapons;



};