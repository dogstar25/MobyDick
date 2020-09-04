#pragma once

#include <cstdint>
#include <array>
#include <bitset>

#pragma warning(push,0)
#include <box2d/box2d.h>
#pragma warning(pop)

#include "Globals.h"

class CollisionManager
{

public:
	CollisionManager();
	~CollisionManager();
	static CollisionManager& instance();

private:





};

