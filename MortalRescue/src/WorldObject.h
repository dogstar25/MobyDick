#pragma once
#include "GameObject.h"

class WeaponObject;

enum worldObjectCategory {
	GENERIC = 1,
	PLAYER = 2,
	WALL = 4,
	PLAYER_BULLET = 8,
	PARTICLE1 = 16,
	PARTICLE2 = 32,
	PARTICLE3 = 64,
	ENEMY_FRAME = 128,
	ENEMY_ARMOR = 256,
	ENEMY_ARMOR_PIECE = 512
};


class WorldObject :	public GameObject
{
public:
	WorldObject();
	WorldObject(string, float, float, float);
	~WorldObject();

	void update() override;
	void render() override;
	void setPosition(b2Vec2, float) override;
	SDL_Rect getRenderDestRect() override; 
	SDL_Rect getPositionRect() override;
	bool testStrength(int);
	virtual void setBox2DUserData(WorldObject*);
	void setActive(bool);
	void setStrength(float strength) { m_strength = strength; }
	void addWeapon(string weaponObjectId);

	
	//Accessor Functions
	b2Body* physicsBody() {
		return m_physicsBody;
	}
	float speed() {
		return m_speed;
	}
	float strength() {
		return m_strength;
	}
	WeaponObject* weapon() {
		return m_weapon;
	}

private:

	b2Body* m_physicsBody;
	float m_speed;
	float m_strength;
	WeaponObject* m_weapon;

	b2Body* buildB2Body(GameObjectDefinition*);
	uint16 setCollisionMask(uint16 category);



};

