#include "IMGuiFactory.h"
#include "IMGuiExample.h"
#include "IMGuiText.h"
#include "IMGuiInteractiveMenuBasic.h"



std::shared_ptr<IMGuiItem> IMGuiFactory::create(std::string iMGuiItemType, std::string gameObjectId, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor, 
	ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize, std::string staticTextValue)
{
	std::shared_ptr<IMGuiItem> iMGuiItem;
	ImVec4 colorNA{0,0,0,0};

	if (iMGuiItemType == "IMGuiExample") {

		iMGuiItem = std::make_shared<IMGuiExample>();
	}
	else if (iMGuiItemType == "IMGuiText") {

		iMGuiItem = std::make_shared<IMGuiText>(gameObjectId, padding, backgroundColor, textColor, colorNA, colorNA, colorNA, autoSize, staticTextValue);
	}
	else if (iMGuiItemType == "IMGuiInteractiveMenuBasic") {

		iMGuiItem = std::make_shared<IMGuiInteractiveMenuBasic>(gameObjectId, padding, backgroundColor, textColor,	buttonColor, buttonHoverColor, buttonActiveColor, autoSize);
	}

	else {

		assert(false && "IMGui Type was not found");
	}

	return iMGuiItem;

}
