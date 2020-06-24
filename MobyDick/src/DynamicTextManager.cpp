#include "DynamicTextManager.h"


DynamicTextManager& DynamicTextManager::instance()
{
	static DynamicTextManager singletonInstance;
	return singletonInstance;

}

DynamicTextManager::DynamicTextManager()
{

	TextItem* item = new TextItem();
	item->textValue = "defaultText";
	item->hasChanged = true;
	m_textItems["DEFAULT"] = std::make_unique<TextItem>(*item);

}


DynamicTextManager::~DynamicTextManager()
{

	m_textItems.clear();
}

TextItem* DynamicTextManager::getTextItem(std::string id)
{
	TextItem* textItem;

	auto iter = m_textItems.find(id);

	if (iter != m_textItems.end())
	{

		textItem = m_textItems[id].get();

	}
	else
	{
		textItem = m_textItems["DEFAULT"].get();
	}

	return textItem;

}


bool DynamicTextManager::updateText(std::string id, std::string newText)
{

	bool wasFound = false;

	//If the text item is already in the map, then update it, otherwise create it
	auto iter = m_textItems.find(id);
	
	if (iter != m_textItems.end())
	{
		if(iter->second->textValue.compare(newText) != 0)
		{
			iter->second->textValue = newText;
			iter->second->hasChanged = true;
		}

		wasFound = true;

	}
	else
	{
		TextItem* item = new TextItem();
		item->textValue = newText;
		item->hasChanged = true;
		m_textItems[id] = std::make_unique<TextItem>(*item);
	}

	return wasFound;

}
