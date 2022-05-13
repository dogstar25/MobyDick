#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <bitset>

#include <box2d/box2d.h>

#include "Util.h"
#include "BaseConstants.h"

class ContactFilter : public b2ContactFilter
{
public:
	ContactFilter();

	bool virtual ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) override;

protected:
	std::vector<std::bitset<ContactTag::MAX_OBJECT_CATEGORIES>> m_contactMasks;

};

