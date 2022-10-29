#ifndef BRAINAABBCALLBACK_H
#define BRAINAABBCALLBACK_H

#include <box2d/box2d.h>
#include <vector>
#include <memory>
#include "GameObject.h"

//class GameObject;

struct BrainAABBFoundObject
{
	b2Fixture* fixture;
	GameObject* gameObject;
};

class BrainAABBCallback : public b2QueryCallback
{

public:
	static BrainAABBCallback& instance();

	std::vector<BrainAABBFoundObject>& foundObjects() { return m_foundObjects; }

	void reset();

private:

	BrainAABBCallback() {};
	bool ReportFixture(b2Fixture* fixture) override;

	std::vector<BrainAABBFoundObject> m_foundObjects;


};

#endif