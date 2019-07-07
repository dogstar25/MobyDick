#include "DebugPanel.h"

#include "TextObject.h"
#include "game.h"


DebugPanel::DebugPanel()
{

	this->location.x = game->config.debugPanelLocation.x;
	this->location.y = game->config.debugPanelLocation.y;

	contentOffset = 1;
}


DebugPanel::~DebugPanel()
{
}

void DebugPanel::addItem(string id, string value)
{
	//Add a seq number to the Id to always make sure that the id is unique
	string newId = id + std::to_string(this->itemCount);

	//Calculate the position of the debug text item
	int xPos = this->location.x + this->contentOffset;
	int yPos = this->location.y += this->itemCount + this->contentOffset;

	TextObject* textObject = new TextObject(newId, xPos, yPos, 0);
	game->addGameObject(textObject, game->DEBUG);

	//Add the dynamic text itself to the dynamic text manager
	game->dynamicTextManager.updateText("newId", value);

	this->itemCount++;



}
