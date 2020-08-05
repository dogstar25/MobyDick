#ifndef GAME_OBJECT_DEFINITION_H
#define GAME_OBJECT_DEFINITION_H

#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <Box2D/Box2D.h>
#include <json/json.h>
#include <vector>


class GameObjectDefinition
{
public:

	GameObjectDefinition();
	GameObjectDefinition(Json::Value gameObjectDefinitionJSON);
	~GameObjectDefinition();

	void setDefinitionJSON(Json::Value definitionJSON) {
		m_definitionJSON = definitionJSON;
	}

	//Accessor functions
	Json::Value definitionJSON() {
		return m_definitionJSON;
	}

private:

	std::string
		m_id;
	Json::Value m_definitionJSON;


};

#endif