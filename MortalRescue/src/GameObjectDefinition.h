#pragma once
#include <string>
#include <map>
#include <SDL.h>

#include "GameObjectAnimation.h"

using namespace std;

struct TextDetails {

	SDL_Color color;
	int size;
	bool isDynamic;
	string fontId;
	string label;

};

class GameObjectDefinition
{
public:

	GameObjectDefinition();
	~GameObjectDefinition();

	string
		id,
		description,
		physicsType,
		collisionShape,
		textureId;
	float
		xSize,
		ySize,
		speed,
		friction,
		density,
		linearDamping,
		collisionRadius,
		angularDamping,
		lifetime;
	bool
		isAnimated,
		isPhysicsObject,
		isPrimitiveShape,
		isPlayerObject,
		isTextObject,
		isParticle;

	TextDetails textDetails;
	SDL_Color primativeColor; //If object is a primative shape, what color is it
	map<string, GameObjectAnimation*> animations;


};