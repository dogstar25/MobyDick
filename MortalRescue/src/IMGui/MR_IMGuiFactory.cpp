#include "MR_IMGuiFactory.h"
#include "IMGuiPauseWindow.h"
#include "IMGuiTopHud.h"
#include "IMGuiInteractiveMenuSurvivor.h"


MR_IMGuiFactory::MR_IMGuiFactory()
{

	int todd = 1;
}


std::shared_ptr<IMGuiItem> MR_IMGuiFactory::create(std::string iMGuiItemType, std::string gameObjectId, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor,
	ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize, std::string staticTextValue)
{
	std::shared_ptr<IMGuiItem> iMGuiItem;

	if (iMGuiItemType == "IMGuiPauseWindow") {
		iMGuiItem = std::make_shared<IMGuiPauseWindow>(gameObjectId, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiTopHud") {
		iMGuiItem = std::make_shared<IMGuiTopHud>(gameObjectId, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiInteractiveMenuSurvivor") {
		iMGuiItem = std::make_shared<IMGuiInteractiveMenuSurvivor>(gameObjectId, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else {

		iMGuiItem = IMGuiFactory::create(iMGuiItemType, gameObjectId, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize, staticTextValue);

	}

	return iMGuiItem;

}
