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
		lifetime;
	bool
		isAnimated,
		isPhysicsObject,
		isPrimitive,
		isPlayerObject,
		isTextObject,
		isParticle,
		isMouseSelectable,
		hasChildObjects,
		absolutePositioning,
		renderOutline;
	uint16
		collisionCategory;

	TextDetails textDetails;
	SDL_Color color; //If object is a primative shape, what color is it
	map<string, Animation*> animations;
	vector<ChildObjectDetails> childObjectDefinitions;



};