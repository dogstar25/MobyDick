#ifndef GAME_OBJECT_DEFINITION_H
#define GAME_OBJECT_DEFINITION_H

#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <Box2D/Box2D.h>

#include <vector>


struct TextDetails {

	SDL_Color color;
	int size;
	bool isDynamic;
	std::string fontId;
	std::string value;

};

struct ChildObjectDetails
{
	std::string gameObjectId;
	short locationSlot;
	std::string gameObjectType;
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
	std::string gameObjectId;
};

/*
Texture file representing the maps of the composite object and the legend that has the info
Of how to decipher the pixels in the blueprint texture
*/
struct CompositeBlueprint
{
	std::string textureId;
	std::vector<CompositeLegendItem> legend;
};

struct CompositeDetails
{
	short maxlevel;
	float levelUpSpeed;
	std::vector<CompositeLevel> levels;
	CompositeBlueprint blueprint;
};

struct AnimationItem
{
	std::string state;
	std::string textureId;
	float speed;
	int frames;

};

struct AnimationDetails
{
	std::vector<AnimationItem> animations;
};

struct WeaponLevelDetails
{
	int level;
	int levelUpTarget;
	int strength;
	SDL_Color color;
	std::string bulletPoolId;
};

struct WeaponDetails
{
	b2Vec2 anchorPoint;
	std::map<int, WeaponLevelDetails> weaponLevelDetails;
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

	std::string
		id,
		type,
		description,
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
		collisionCategory,
		collisionShape,
		physicsType ;
	SDL_Color 
		color;
	b2Vec2 weaponAnchorPoint;

	/*
	Complex definition details
	*/
	TextDetails textDetails;
	CompositeDetails compositeDetails;
	AnimationDetails animationDetails;
	WeaponDetails weaponDetails;
	

	std::vector<ChildObjectDetails> childObjectDefinitions; //TODO:turn this into ChildDetails

};

#endif