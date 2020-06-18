#pragma once
#include <string>

#include <Box2D/Box2D.h>
#include <SDL2/SDL_image.h>

#include "WorldObject.h"




class WeaponObject :
	public WorldObject
{
public:
	WeaponObject(string gameObjectId, float xMapPos, float yMapPos, float angleAdjust);
	~WeaponObject();

	void fire(const b2Vec2 &origin, const float &angle, const float &fireOffset);
	bool checkLevelUp(const int &pieceCount);
	void levelUp();

private:
	int	m_currentLevel;

};

