#include "DynamicTextManager.h"


DynamicTextManager& DynamicTextManager::instance()
{
	static DynamicTextManager singletonInstance;
	return singletonInstance;

}

DynamicTextManager::DynamicTextManager()
{

	textItem* item = new textItem();
	item->text = "defaultText";
	item->hasChanged = true;
	this->textItems["DEFAULT"] = std::make_unique<textItem>(*item);

}


DynamicTextManager::~DynamicTextManager()
{

	this->textItems.clear();
}

textItem* DynamicTextManager::getTextItem(std::string id)
{
	textItem* textItem;

	auto iter = this->textItems.find(id);

	if (iter != this->textItems.end())
	{

		textItem = this->textItems[id].get();

	}
	else
	{
		textItem = this->textItems["DEFAULT"].get();
	}

	return textItem;

}


bool DynamicTextManager::updateText(std::string id, std::string newText)
{

	bool wasFound = false;

	//If the text item is already in the map, then update it, otherwise create it
	auto iter = this->textItems.find(id);
	
	if (iter != this->textItems.end())
	{
		if(iter->second->text.compare(newText) != 0)
		{
			iter->second->text = newText;
			iter->second->hasChanged = true;
		}

		wasFound = true;

	}
	else
	{
		textItem* item = new textItem();
		item->text = newText;
		item->hasChanged = true;
		this->textItems[id] = std::make_unique<textItem>(*item);
	}

	return wasFound;

}
