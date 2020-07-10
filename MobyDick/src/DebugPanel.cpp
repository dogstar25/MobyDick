#include "DebugPanel.h"

#include "Globals.h"
#include "DynamicTextManager.h"
#include "GameObjectManager.h"
#include "GameConfig.h"
#include "TextObject.h"
#include "Level.h"
#include "Game.h"


DebugPanel& DebugPanel::instance()
{
	static DebugPanel singletonInstance;
	return singletonInstance;
}

DebugPanel::DebugPanel()
{

	m_location.x = GameConfig::instance().debugPanelLocation().x;
	m_location.y = GameConfig::instance().debugPanelLocation().y;

	m_itemCount = 0;

}


DebugPanel::~DebugPanel()
{
}

void DebugPanel::addItem(std::string id, float value, int decDigits)
{
	std::string debugValue = util::floatToString(value, decDigits);
	DebugPanel::addItem(id, debugValue);

}

void DebugPanel::addItem(std::string id, std::string value)
{

	if (GameConfig::instance().debugPanel() == true) {

		float yPos, xPos, fontSize;

		//Prefix DEBUG to the id
		std::string newId = "DEBUG_" + id;

		//Build the new text using the id as a label
		std::string newText = id + " : " + value;

		//Add or update the dynamic text itself to the dynamic text manager
		bool alreadyExists = DynamicTextManager::instance().updateText(newId, newText);

		if (alreadyExists == false)
		{
			if (GameObjectManager::instance().hasDefinition("DEBUG_ITEM"))
			{
				Json::Value definitionJSON = GameObjectManager::instance().getDefinition("DEBUG_ITEM")->definitionJSON();
				fontSize = definitionJSON["textComponent"]["fontSize"].asFloat();
			}
			else
			{
				fontSize = 12;
			}
			

			//Calculate the position of the debug text item
			xPos = m_location.x;
			//FIXME: Level tile size 32
			yPos = m_location.y + m_itemCount * (fontSize / (float)32);

			Game::instance().addGameObject(newId, GameObjectLayer::DEBUG, xPos, yPos, 0);

			m_itemCount++;
		}

	}
}
