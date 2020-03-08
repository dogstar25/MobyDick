#include "DebugPanel.h"
#include "Util.h"
#include "DynamicTextManager.h"
#include "GameObjectManager.h"
#include "GameConfig.h"
#include "Game.h"






DebugPanel::DebugPanel()
{

	this->location.x = GameConfig::instance().debugPanelLocation().x;
	this->location.y = GameConfig::instance().debugPanelLocation().y;

	this->itemCount = 0;

}


DebugPanel::~DebugPanel()
{
}

void DebugPanel::addItem(string id, float value, int decDigits)
{
	string debugValue = util::floatToString(value, decDigits);
	DebugPanel::addItem(id, debugValue);

}

void DebugPanel::addItem(string id, string value)
{

	if (GameConfig::instance().debugPanel() == true) {

		float yPos, xPos;

		//Get the base definition for debug text items
		GameObjectDefinition* definition = GameObjectManager::instance().gameObjectDefinitions["DEBUG_ITEM"];;

		//Prefix DEBUG to the id
		string newId = "DEBUG_" + id;

		//Build the new text using the id as a label
		string newText = id + " : " + value;

		//Add or update the dynamic text itself to the dynamic text manager
		bool alreadyExists = DynamicTextManager::instance().updateText(newId, newText);

		if (alreadyExists == false)
		{

			//Calculate the position of the debug text item
			xPos = this->location.x;
			//yPos = this->location.y + this->itemCount * (.5);
			yPos = this->location.y + this->itemCount * ((float)definition->textDetails.size / (float)game->worldGridSize.h);

			TextObject* textObject = GameObjectManager::instance().buildGameObject <TextObject>(newId, xPos, yPos, 0);
			game->addGameObject(textObject, GameOjectLayer::DEBUG);

			this->itemCount++;
		}

	}
}
