#include "DynamicTextManager.h"



DynamicTextManager::DynamicTextManager()
{


}


DynamicTextManager::~DynamicTextManager()
{

	this->textItems.clear();
}

textItem* DynamicTextManager::getTextItem(string id)
{
	textItem* textItem;

	if (this->textItems[id] == NULL)
	{

		//createa  new default text item???

	}
	else
	{
		textItem = this->textItems[id].get();
	}

	return textItem;

}


void DynamicTextManager::updateText(string id, string newText)
{

	//If the text item is already in the map, then update it, otherwise create it
	auto iter = this->textItems.find(id);
	
	if (iter != this->textItems.end())
	{
		if(iter->second->text.compare(newText) != 0)
		{
			iter->second->text = newText;
			iter->second->hasChanged = true;
		}
		else
		{
			iter->second->hasChanged = false;
		}

	}
	else
	{
		textItem* item = new textItem();
		item->text = newText;
		item->hasChanged = true;
		this->textItems[id] = make_unique<textItem>(*item);
	}

}
