#pragma once
#include <string>
#include <map>
#include <SDL.h>
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
		restitution,
		density,
		linearDamping,
		collisionRadius,
		angularDamping,
		lifetime;
	bool
		isAnimated,
		isPhysicsObject,
		isPrimitive,
		isPlayerObject,
		isTextObject,
		isParticle,
		absolutePositioning;
	short
		collisionGroup; // objects with same negative group value will not collide

	TextDetails textDetails;
	SDL_Color color; //If object is a primative shape, what color is it
	map<string, Animation*> animations;


};