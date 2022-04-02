#include "MR_IMGuiFactory.h"
#include "IMGuiPauseWindow.h"
#include "IMGuiTopHud.h"


MR_IMGuiFactory::MR_IMGuiFactory()
{

	int todd = 1;
}


std::shared_ptr<IMGuiItem> MR_IMGuiFactory::create(std::string iMGuiItemType, std::string gameObjectId, b2Vec2 padding, ImVec4 color, bool autoSize, std::string staticTextValue)
{
	std::shared_ptr<IMGuiItem> iMGuiItem;

	if (iMGuiItemType == "IMGuiPauseWindow") {
		iMGuiItem = std::make_shared<IMGuiPauseWindow>(gameObjectId, padding, color, autoSize);
	}
	else if (iMGuiItemType == "IMGuiTopHud") {
		iMGuiItem = std::make_shared<IMGuiTopHud>(gameObjectId, padding, color, autoSize);
	}
	
	else {

		iMGuiItem = IMGuiFactory::create(iMGuiItemType, gameObjectId, padding, color, autoSize, staticTextValue);

	}

	return iMGuiItem;

}
