#ifndef GAME_OBJECT_DEFINITION_H
#define GAME_OBJECT_DEFINITION_H

#include <string>
#include <map>
#include <SDL2/SDL.h>
#pragma warning(push,0)
#include <box2d/box2d.h>
#include <json/json.h>
#pragma warning(pop)
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

	std::string	m_id;
	Json::Value m_definitionJSON;


};

#endif