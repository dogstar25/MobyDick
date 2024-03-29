#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <bitset>

#include <box2d/box2d.h>

#include "ContactFilter.h"
#include "Util.h"
#include "BaseConstants.h"

class BBContactFilter : public ContactFilter
{
public:
	BBContactFilter();
	bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) final;

};

