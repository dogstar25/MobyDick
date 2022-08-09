#include "GameObjectDefinition.h"



GameObjectDefinition::GameObjectDefinition()
{

}

GameObjectDefinition::GameObjectDefinition(Json::Value gameObjectDefinitionJSON)
{

	m_id = gameObjectDefinitionJSON["id"].asString();

	m_definitionJSON = gameObjectDefinitionJSON;



	//gameObjectDefinition->fireOffset = itr["fireOffset"].asFloat();


	////Game Object Type
	//gameObjectDefinition->type = itr["type"].asString();


	////If this is a physics object then build the box2d body
	//if (itr["physicsObject"].isNull() == false)
	//{
	//	gameObjectDefinition->isPhysicsObject = true;
	//	gameObjectDefinition->friction = itr["physicsObject"]["friction"].asFloat();
	//	gameObjectDefinition->restitution = itr["physicsObject"]["restitution"].asFloat();
	//	gameObjectDefinition->density = itr["physicsObject"]["density"].asFloat();
	//	gameObjectDefinition->linearDamping = itr["physicsObject"]["linearDamping"].asFloat();
	//	gameObjectDefinition->angularDamping = itr["physicsObject"]["angularDamping"].asFloat();
	//	gameObjectDefinition->physicsType = game->enumMap()->toEnum(itr["physicsObject"]["type"].asString());
	//	gameObjectDefinition->collisionShape = game->enumMap()->toEnum(itr["physicsObject"]["collisionShape"].asString());
	//	gameObjectDefinition->collisionRadius = itr["physicsObject"]["collisionRadius"].asFloat();

	//	gameObjectDefinition->collisionCategory = game->enumMap()->toEnum(itr["physicsObject"]["collisionCategory"].asString());
	//	gameObjectDefinition->weaponAnchorPoint.x = itr["physicsObject"]["weaponAnchorPoint"]["x"].asFloat();
	//	gameObjectDefinition->weaponAnchorPoint.y = itr["physicsObject"]["weaponAnchorPoint"]["y"].asFloat();

	//}

	////If this object is a selectable or clickable object
	//if (itr["uiEvents"].isNull() == false)
	//{
	//	gameObjectDefinition->isMouseSelectable = true;
	//	gameObjectDefinition->onClickAction = itr["uiEvents"]["onClickAction"].asString();

	//}
	////If this is a text object then store text details
	//if (itr["text"].isNull() == false)
	//{
	//	gameObjectDefinition->isTextObject = true;
	//	gameObjectDefinition->textDetails.value = itr["text"]["value"].asString();
	//	gameObjectDefinition->textDetails.fontId = itr["text"]["font"].asString();
	//	gameObjectDefinition->textDetails.isDynamic = itr["text"]["dynamic"].asBool();
	//	gameObjectDefinition->textDetails.size = itr["text"]["size"].asInt();
	//	if (itr["text"]["color"].isNull() == false)
	//	{
	//		gameObjectDefinition->textDetails.color.a = itr["text"]["color"]["alpha"].asInt();
	//		gameObjectDefinition->textDetails.color.r = itr["text"]["color"]["red"].asInt();
	//		gameObjectDefinition->textDetails.color.g = itr["text"]["color"]["green"].asInt();
	//		gameObjectDefinition->textDetails.color.b = itr["text"]["color"]["blue"].asInt();
	//	}
	//	else
	//	{
	//		gameObjectDefinition->textDetails.color.a = 255;
	//		gameObjectDefinition->textDetails.color.r = 255;
	//		gameObjectDefinition->textDetails.color.g = 255;
	//		gameObjectDefinition->textDetails.color.b = 255;

	//	}

	//}

	////Child Objects
	//gameObjectDefinition->childPadding = itr["childPadding"].asFloat();
	//gameObjectDefinition->childPositionRelative = itr["childPositionRelative"].asBool();
	//if (itr["childObjects"].isNull() == false)
	//{
	//	gameObjectDefinition->hasChildObjects = true;
	//	ChildObjectDetails childDetails;
	//	for (auto childItr : itr["childObjects"])
	//	{
	//		childDetails.gameObjectId = childItr["gameObjectId"].asString();
	//		childDetails.locationSlot = childItr["locationSlot"].asInt();
	//		childDetails.gameObjectType = childItr["gameObjectType"].asString();
	//		gameObjectDefinition->childObjectDefinitions.push_back(childDetails);
	//	}
	//}

	////CompositeObject Details
	//if (itr["composite"].isNull() == false)
	//{
	//	gameObjectDefinition->isCompositeObject = true;
	//	gameObjectDefinition->compositeDetails.levelUpSpeed = itr["composite"]["levelUpSpeed"].asFloat();

	//	//Composite bluprint textureId
	//	gameObjectDefinition->compositeDetails.blueprint.textureId =
	//		itr["composite"]["blueprint"]["texture"].asString();

	//	//loop though all legend items that define how the composite should be built
	//	CompositeLegendItem legendItem;
	//	for (auto legendItr : itr["composite"]["blueprint"]["legend"])
	//	{
	//		//GameObjectId
	//		legendItem.gameObjectId = legendItr["gameObjectId"].asString();

	//		//color
	//		legendItem.color.r = legendItr["color"]["red"].asInt();
	//		legendItem.color.g = legendItr["color"]["green"].asInt();
	//		legendItem.color.b = legendItr["color"]["blue"].asInt();
	//		legendItem.color.a = legendItr["color"]["alpha"].asInt();

	//		gameObjectDefinition->compositeDetails.blueprint.legend.push_back(legendItem);

	//	}

	//	//CompositeLegendItem legendItem;
	//	CompositeLevel compositeLevel;
	//	for (auto levelItr : itr["composite"]["levels"])
	//	{
	//		compositeLevel.levelNum = levelItr["level"].asInt();
	//		compositeLevel.force = levelItr["force"].asInt();

	//		compositeLevel.color.r = levelItr["color"]["red"].asInt();
	//		compositeLevel.color.g = levelItr["color"]["green"].asInt();
	//		compositeLevel.color.b = levelItr["color"]["blue"].asInt();
	//		compositeLevel.color.a = levelItr["color"]["alpha"].asInt();

	//		gameObjectDefinition->compositeDetails.levels.push_back(compositeLevel);

	//	}
	//}

	///*
	//Weapons details
	//*/
	//float x = itr["physicsObject"]["anchorPoint"]["x"].asFloat();
	//float y = itr["physicsObject"]["anchorPoint"]["y"].asFloat();
	//gameObjectDefinition->weaponDetails.anchorPoint.Set(x, y);
	//WeaponLevelDetails weaponLevelDetails;
	//for (auto weaponItr : itr["weaponLevels"])
	//{
	//	weaponLevelDetails.level = weaponItr["level"].asInt();
	//	weaponLevelDetails.levelUpTarget = weaponItr["levelUpTarget"].asInt();
	//	weaponLevelDetails.force = weaponItr["force"].asInt();
	//	weaponLevelDetails.bulletPoolId = weaponItr["bulletPoolId"].asString();
	//	weaponLevelDetails.color.r = weaponItr["color"]["red"].asInt();
	//	weaponLevelDetails.color.g = weaponItr["color"]["green"].asInt();
	//	weaponLevelDetails.color.b = weaponItr["color"]["blue"].asInt();
	//	weaponLevelDetails.color.a = weaponItr["color"]["alpha"].asInt();
	//	gameObjectDefinition->weaponDetails.weaponLevelDetails.emplace(weaponLevelDetails.level, weaponLevelDetails);

	//}

	//this->gameObjectDefinitions[gameObjectDefinition->id] = gameObjectDefinition;



}


GameObjectDefinition::~GameObjectDefinition()
{
}
