#ifndef WEAPON_COMPONENT_H
#define WEAPON_COMPONENT_H

#include<string>
#include <box2d/box2d.h>
#include <json/json.h>
#include <SDL2/SDL.h>
#include <optional>
#include "Component.h"

class GameObject;

struct WeaponLevelDetail {

	int level{};
	float force{};
	int levelUpTarget{};
	std::optional<SDL_Color> color{};
	std::string bulletPoolId{};
};

class WeaponComponent : public Component
{

public:
	WeaponComponent();
	WeaponComponent(Json::Value componentJSON);
	~WeaponComponent();

	void update();
	void fire(const b2Vec2& origin, const float& angle);
	void checkLevelUp() {};
	void checkLevelUp(const int) {};


protected:
	
	void _levelUp();
	
	int	m_maxLevel{ 1 };
	int m_currentLevel{ 1 };
	float
		m_fireOffset;

	std::map<int, WeaponLevelDetail> m_weaponLevelDetails;


};


#endif

