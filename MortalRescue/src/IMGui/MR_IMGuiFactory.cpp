#include "MR_IMGuiFactory.h"
#include "IMGuiSettings.h"
#include "IMGuiPauseWindow.h"
#include "IMGuiTopHud.h"


MR_IMGuiFactory::MR_IMGuiFactory()
{

	int todd = 1;
}


std::shared_ptr<IMGuiItem> MR_IMGuiFactory::create(std::string iMGuiItemType, std::string gameObjectName, Json::Value parms)
{
	std::shared_ptr<IMGuiItem> iMGuiItem;

	if (iMGuiItemType == "IMGuiSettings") {

		iMGuiItem = std::make_shared<IMGuiSettings>();
	}
	else if (iMGuiItemType == "IMGuiPauseWindow") {
		iMGuiItem = std::make_shared<IMGuiPauseWindow>(parms, gameObjectName);
	}
	else if (iMGuiItemType == "IMGuiTopHud") {
		iMGuiItem = std::make_shared<IMGuiTopHud>(parms, gameObjectName);
	}
	
	else {

		iMGuiItem = IMGuiFactory::create(iMGuiItemType, gameObjectName, parms);

	}

	return iMGuiItem;

}
