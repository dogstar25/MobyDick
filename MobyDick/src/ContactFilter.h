#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <bitset>

#pragma warning(push,0)
#include <box2d/box2d.h>
#pragma warning(pop)

#include "Globals.h"
#include "BaseConstants.h"

class ContactFilter : public b2ContactFilter
{
public:
	ContactFilter();
	~ContactFilter();

	static ContactFilter& instance();
	bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) override;

private:
	std::vector<std::bitset<IdTag::MAX_OBJECT_CATEGORIES>> m_collisionMasks;

};

