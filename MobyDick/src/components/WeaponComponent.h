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
	float m_fireOffset{};
	float m_currentLevelUpAccrual{};

	//Even though the levelUpIncrement is the same for each level,
	//Individual games can adjust the collection item value by a percentage to make each level different
	//This woul dbe done at the individual games' contactlistener collision logic
	float m_levelUpIncrement{};

	std::map<int, WeaponLevelDetail> m_weaponLevelDetails;


};


#endif

