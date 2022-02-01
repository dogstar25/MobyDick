#pragma once
#include <memory.h>

#include "CutScene.h"

class CutSceneFactory
{
public:
	
	std::shared_ptr<CutScene> create(std::string cutSceneType);

private:

};
