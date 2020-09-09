#ifndef WEAPON_COMPONENT_H
#define WEAPON_COMPONENT_H

#include<string>
#include <box2d/box2d.h>
#include <json/json.h>
#include <SFML/Graphics.hpp>
#include <SDL2/SDL.h>

#include "Component.h"

class GameObject;

struct WeaponLevelDetail {

	int level;
	float force;
	int levelUpTarget;
	sf::Color color;
	std::string bulletPoolId;
};

class WeaponComponent : public Component
{

public:
	WeaponComponent();
	WeaponComponent(Json::Value definitionJSON);
	~WeaponComponent();

	void update();
	void fire(const sf::Vector2f& origin, const float& angle);
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

