#include "IMGuiFactory.h"
#include "IMGuiExample.h"
#include "IMGuiText.h"
#include "IMGuiHintBasic.h"



std::shared_ptr<IMGuiItem> IMGuiFactory::create(std::string iMGuiItemType, std::string gameObjectId, b2Vec2 padding, ImVec4 color, bool autoSize, std::string staticTextValue)
{
	std::shared_ptr<IMGuiItem> iMGuiItem;

	if (iMGuiItemType == "IMGuiExample") {

		iMGuiItem = std::make_shared<IMGuiExample>();
	}
	else if (iMGuiItemType == "IMGuiText") {

		iMGuiItem = std::make_shared<IMGuiText>(gameObjectId, padding, color, autoSize, staticTextValue);
	}
	else if (iMGuiItemType == "IMGuiHintBasic") {

		iMGuiItem = std::make_shared<IMGuiHintBasic>(gameObjectId, padding, color, autoSize);
	}

	else {

		assert(false && "IMGui Type was not found");
	}

	return iMGuiItem;

}
