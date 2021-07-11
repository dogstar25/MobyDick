#pragma once
#include <box2d/box2d.h>
#include <vector>

class GameObject;


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

