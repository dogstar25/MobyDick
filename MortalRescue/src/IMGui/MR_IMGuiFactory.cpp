#include "MR_IMGuiFactory.h"
#include "IMGuiPauseWindow.h"
#include "IMGuiInteractiveMenuSurvivor.h"
#include "IMGuiTopHud.h"
#include "IMGuiTopLeftHud.h"
#include "IMGuiTopRightHud.h"
#include "IMGuiLevelComplete.h"


MR_IMGuiFactory::MR_IMGuiFactory()
{

}


std::shared_ptr<IMGuiItem> MR_IMGuiFactory::create(std::string iMGuiItemType, std::string gameObjectId, Scene* parentScene, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor,
	ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize, std::string staticTextValue)
{
	std::shared_ptr<IMGuiItem> iMGuiItem;

	if (iMGuiItemType == "IMGuiPauseWindow") {
		iMGuiItem = std::make_shared<IMGuiPauseWindow>(gameObjectId, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiTopHud") {
		iMGuiItem = std::make_shared<IMGuiTopHud>(gameObjectId, padding, parentScene, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiTopLeftHud") {
		iMGuiItem = std::make_shared<IMGuiTopLeftHud>(gameObjectId, padding, parentScene, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiTopRightHud") {
		iMGuiItem = std::make_shared<IMGuiTopRightHud>(gameObjectId, padding, parentScene, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiInteractiveMenuSurvivor") {
		iMGuiItem = std::make_shared<IMGuiInteractiveMenuSurvivor>(gameObjectId, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiLevelComplete") {
		iMGuiItem = std::make_shared<IMGuiLevelComplete>(gameObjectId, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}

	else {

		iMGuiItem = IMGuiFactory::create(iMGuiItemType, gameObjectId, parentScene, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize, staticTextValue);

	}

	return iMGuiItem;

}
