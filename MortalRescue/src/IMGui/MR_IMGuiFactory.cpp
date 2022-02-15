#include "MR_IMGuiFactory.h"
#include "IMGuiSettings.h"


MR_IMGuiFactory::MR_IMGuiFactory()
{

	int todd = 1;
}


std::shared_ptr<IMGuiItem> MR_IMGuiFactory::create(std::string iMGuiItemType)
{
	std::shared_ptr<IMGuiItem> iMGuiItem;

	if (iMGuiItemType == "IMGuiSettings") {

		iMGuiItem = std::make_shared<IMGuiSettings>();
	}
	else {

		iMGuiItem = IMGuiFactory::create(iMGuiItemType);

	}

	return iMGuiItem;

}
