#pragma once
#include <memory.h>

#include "cutScenes/CutScene.h"
#include "cutScenes/CutSceneFactory.h"

class BBCutSceneFactory : public CutSceneFactory
{
public:
	
	std::shared_ptr<CutScene> create(std::string cutSceneType);

private:

};
