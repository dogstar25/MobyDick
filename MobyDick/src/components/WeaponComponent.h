#ifndef WEAPON_COMPONENT_H
#define WEAPON_COMPONENT_H

#include<string>
#include <box2d/box2d.h>

#include "Component.h"

class WeaponComponent : public Component
{

public:
	WeaponComponent();
	WeaponComponent(std::string gameObjectId);
	WeaponComponent(std::string gameObjectId, float xMapPos, float yMapPos, float angleAdjust);
	~WeaponComponent();

	void update();
	void fire(const b2Vec2& origin, const float& angle, const float& fireOffset);
	bool checkLevelUp(const int& pieceCount);
	void levelUp();


private:
	int	m_currentLevel;


};


#endif

