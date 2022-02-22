#include "IMGuiFactory.h"
#include "IMGuiExample.h"
#include "IMGuiText.h"



std::shared_ptr<IMGuiItem> IMGuiFactory::create(std::string iMGuiItemType, std::string gameObjectName, Json::Value parms)
{
	std::shared_ptr<IMGuiItem> iMGuiItem;

	if (iMGuiItemType == "IMGuiExample") {

		iMGuiItem = std::make_shared<IMGuiExample>(parms, gameObjectName);
	}
	else if (iMGuiItemType == "IMGuiText") {

		iMGuiItem = std::make_shared<IMGuiText>(parms, gameObjectName);
	}

	else {

		assert(false && "IMGui Type was not found");
	}

	return iMGuiItem;

}
