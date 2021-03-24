#include "DebugPanel.h"

#include "Globals.h"
#include "game.h"
#include "DynamicTextManager.h"
#include "GameObjectManager.h"
#include "GameConfig.h"
#include "Level.h"


DebugPanel& DebugPanel::instance()
{
	static DebugPanel singletonInstance;
	return singletonInstance;
}

DebugPanel::DebugPanel()
{

	m_location.x = (float)GameConfig::instance().debugPanelLocation().x;
	m_location.y = (float)GameConfig::instance().debugPanelLocation().y;

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
			yPos = m_location.y + m_itemCount * (fontSize / (float)Level::instance().m_tileHeight);

			Game::instance().addGameObject(newId, LAYER_MENU, xPos, yPos, 0);


			m_itemCount++;
		}

	}
}
