#include "WorldObject.h"

#include "game.h"



WorldObject::WorldObject()
{
}


WorldObject::~WorldObject()
{
}


void WorldObject::update()
{
	GameObject::update();

}

void WorldObject::render()
{

	Game::textureManager.renderWorldObject(this);
}
