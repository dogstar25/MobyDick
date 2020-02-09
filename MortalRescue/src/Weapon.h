#pragma once
#include <string>

#include <Box2D/Box2D.h>

using namespace std;

class GameObject;
class PlayerObject;
class Game;

class Weapon
{
private:
	string 
		bulletPoolId;
	float 
		strength;
	int 
		levelUpTarget, 
		level;

public:
	Weapon(string bulletPoolId, float strength, int levelUpTarget, int level);
	~Weapon();

	void fire(b2Vec2 origin, float angle, float fireOffset);


};

