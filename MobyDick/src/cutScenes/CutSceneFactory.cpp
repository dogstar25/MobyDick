#include "CutSceneFactory.h"
#include "CutSceneExample.h"

std::shared_ptr<CutScene> CutSceneFactory::create(std::string cutSceneType)
{
	std::shared_ptr<CutScene> cutScene;

	if (cutSceneType == "CutSceneExample") {

		cutScene = std::make_shared<CutSceneExample>();
	}
	else {

		assert(false && "CuteScene Types was not found");
	}

	return cutScene;

}
