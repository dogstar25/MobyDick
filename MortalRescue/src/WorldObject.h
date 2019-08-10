#pragma once
#include "GameObject.h"


class WorldObject :	public GameObject
{
public:
	WorldObject();
	WorldObject(string,int,int,int);
	~WorldObject();

	void update();
	void render();
	SDL_Rect*  getRenderDestRect(SDL_Rect*);

	b2Body* buildB2Body(GameObjectDefinition*);

	b2Body* physicsBody;

	float speed;
};

