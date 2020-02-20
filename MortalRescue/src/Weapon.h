#pragma once
#include <string>

#include <Box2D/Box2D.h>

class GameObject;
class PlayerObject;
class Game;

class Weapon
{
private:
	std::string 
		m_bulletPoolId;
	float 
		m_strength;
	int 
		m_levelUpTarget, 
		m_level;

public:
	Weapon(std::string bulletPoolId, float strength, int levelUpTarget, int level);
	~Weapon();

	void fire(const b2Vec2 &origin, const float &angle, const float &fireOffset);
	bool checkLevelUp(const int &pieceCount);
	int getNextLevel();


};

