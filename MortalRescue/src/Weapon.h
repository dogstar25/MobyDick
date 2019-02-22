#pragma once
#include <string>

using namespace std;

class GameObject;

class Weapon
{
public:
	Weapon();
	~Weapon();

	void fire();
	void init(string, GameObject*, float,float);


	GameObject* weaponWieldingObject;
	string bulletGameObjectId;
	float xOffset, yOffset; //what offset from the weapon holding obejct to originate bullet from

};

