#ifndef GAME_OBJECT_DEFINITION_H
#define GAME_OBJECT_DEFINITION_H

#include <string>
#include <map>
#include <SDL2/SDL.h>
#include <Box2D/Box2D.h>
#include <json/json.h>
#include <vector>

#include "components/Component.h"


class GameObjectDefinition
{
public:

	GameObjectDefinition(Json::Value gameObjectDefinitionJSON);
	~GameObjectDefinition();

private:

	std::string
		id,
		description;
	unsigned int componentFlags;

	std::map<std::string, Component> components;

};

#endif