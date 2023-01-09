#include "MR_IMGuiFactory.h"
#include "IMGuiPauseWindow.h"
#include "IMGuiInteractiveMenuSurvivor.h"
#include "IMGuiTopHud.h"
#include "IMGuiTopLeftHud.h"
#include "IMGuiTopRightHud.h"
#include "IMGuiLevelComplete.h"
#include "IMGuiPlayerDeath.h"
#include "IMGuiTitleScreenMenu.h"
#include "IMGuiTestLevelMenu.h"
#include "IMGuiYouWin.h"


MR_IMGuiFactory::MR_IMGuiFactory()
{

}


std::shared_ptr<IMGuiItem> MR_IMGuiFactory::create(std::string iMGuiItemType, std::string gameObjectType, Scene* parentScene, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor,
	ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize, std::string staticTextValue)
{
	std::shared_ptr<IMGuiItem> iMGuiItem;

	if (iMGuiItemType == "IMGuiPauseWindow") {
		iMGuiItem = std::make_shared<IMGuiPauseWindow>(gameObjectType, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiTopHud") {
		iMGuiItem = std::make_shared<IMGuiTopHud>(gameObjectType, padding, parentScene, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiTopLeftHud") {
		iMGuiItem = std::make_shared<IMGuiTopLeftHud>(gameObjectType, padding, parentScene, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiTopRightHud") {
		iMGuiItem = std::make_shared<IMGuiTopRightHud>(gameObjectType, padding, parentScene, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiInteractiveMenuSurvivor") {
		iMGuiItem = std::make_shared<IMGuiInteractiveMenuSurvivor>(gameObjectType, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiLevelComplete") {
		iMGuiItem = std::make_shared<IMGuiLevelComplete>(gameObjectType, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiPlayerDeath") {
		iMGuiItem = std::make_shared<IMGuiPlayerDeath>(gameObjectType, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiTitleScreenMenu") {
		iMGuiItem = std::make_shared<IMGuiTitleScreenMenu>(gameObjectType, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiTestLevelMenu") {
		iMGuiItem = std::make_shared<IMGuiTestLevelMenu>(gameObjectType, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}
	else if (iMGuiItemType == "IMGuiYouWin") {
		iMGuiItem = std::make_shared<IMGuiYouWin>(gameObjectType, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}

	else {

		iMGuiItem = IMGuiFactory::create(iMGuiItemType, gameObjectType, parentScene, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize, staticTextValue);

	}

	return iMGuiItem;

}
