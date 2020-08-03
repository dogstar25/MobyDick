#ifndef WEAPON_COMPONENT_H
#define WEAPON_COMPONENT_H

#include<string>
#include <box2d/box2d.h>
#include <SDL2/SDL.h>
#include <json/json.h>

#include "Component.h"

class GameObject;

struct WeaponLevelDetail {

	int level;
	int strength;
	int levelUpTarget;
	SDL_Color color;
	std::string bulletPoolId;
};

class WeaponComponent : public Component
{

public:
	WeaponComponent();
	WeaponComponent(Json::Value definitionJSON);
	WeaponComponent(Json::Value definitionJSON, float xMapPos, float yMapPos, float angleAdjust);
	~WeaponComponent();

	void update();
	void fire(const b2Vec2& origin, const float& angle, const float& fireOffset);
	bool checkLevelUp(const int& pieceCount);
	void levelUp();


private:
	int	
		m_currentLevel;
	float
		m_fireOffset;

	std::map<int, WeaponLevelDetail> m_weaponLevelDetails;


};


#endif

