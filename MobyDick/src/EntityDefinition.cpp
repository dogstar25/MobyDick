#include "EntityDefinition.h"



EntityDefinition::EntityDefinition()
{

}

EntityDefinition::EntityDefinition(Json::Value gameObjectDefinitionJSON)
{

	m_id = gameObjectDefinitionJSON["id"].asString();

	m_definitionJSON = gameObjectDefinitionJSON;



}


EntityDefinition::~EntityDefinition()
{
}
