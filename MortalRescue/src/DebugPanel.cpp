#include "DebugPanel.h"

#include "TextObject.h"
#include "game.h"


DebugPanel::DebugPanel()
{

	this->location.x = game->config.debugPanelLocation.x;
	this->location.y = game->config.debugPanelLocation.y;

	this->itemCount = 0;

}


DebugPanel::~DebugPanel()
{
}

void DebugPanel::addItem(string id, string value)
{

	int yPos, xPos;

	//Prefix DEBUG to the id
	string newId = "DEBUG_" + id;

	//Build the new text using the id as a label
	string newText = id + " : " + value;

	//Add or update the dynamic text itself to the dynamic text manager
	bool alreadyExists = game->dynamicTextManager.updateText(newId, newText);

	if (alreadyExists == false)
	{
		//Calculate the position of the debug text item
		xPos = this->location.x;
		yPos = this->location.y += this->itemCount;

		TextObject* textObject = new TextObject(newId, xPos, yPos, 0);
		game->addGameObject(textObject, game->DEBUG);

		this->itemCount++;
	}


}
