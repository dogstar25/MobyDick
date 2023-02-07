#include "BB_IMGuiFactory.h"
#include "IMGuiPauseWindow.h"
#include "IMGuiTitleScreenMenu.h"
#include "IMGuiTestLevelMenu.h"
#include "IMGuiInteractiveMenuDoor.h"


BB_IMGuiFactory::BB_IMGuiFactory()
{

}


std::shared_ptr<IMGuiItem> BB_IMGuiFactory::create(std::string iMGuiItemType, std::string gameObjectType, Scene* parentScene, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor,
	ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize, std::string staticTextValue)
{
	std::shared_ptr<IMGuiItem> iMGuiItem;

	if (iMGuiItemType == "IMGuiPauseWindow") {
		iMGuiItem = std::make_shared<IMGuiPauseWindow>(gameObjectType, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiTitleScreenMenu") {
		iMGuiItem = std::make_shared<IMGuiTitleScreenMenu>(gameObjectType, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiTestLevelMenu") {
		iMGuiItem = std::make_shared<IMGuiTestLevelMenu>(gameObjectType, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiInteractiveMenuDoor") {
		iMGuiItem = std::make_shared<IMGuiInteractiveMenuDoor>(gameObjectType, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else {

		iMGuiItem = IMGuiFactory::create(iMGuiItemType, gameObjectType, parentScene, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize, staticTextValue);

	}

	return iMGuiItem;

}
