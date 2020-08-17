#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <bitset>

#include <box2d/box2d.h>

#include "Globals.h"

class ContactFilter : public b2ContactFilter
{
public:
	ContactFilter();
	~ContactFilter();

	static ContactFilter& ContactFilter::instance();
	bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) override;

private:
	std::vector<std::bitset<ObjectCategory::MAX_OBJECT_CATEGORIES>> m_collisionMasks;

};

