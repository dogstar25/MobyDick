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
	void setPosition(b2Vec2, float);
	SDL_Rect  getRenderDestRect();

	b2Body* buildB2Body(GameObjectDefinition*);

	b2Body* physicsBody;

	float speed;
};

