#include "GameObjectManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <json/json.h>

#include <iostream>
#include <fstream>

#include "TextureManager.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "WorldObject.h"
#include "TextObject.h"
#include "CompositeObject.h"
#include "Util.h"


GameObjectManager& GameObjectManager::instance()
{
	static GameObjectManager singletonInstance;
	return singletonInstance;
}


GameObjectManager::GameObjectManager()
{

}

GameObjectManager::~GameObjectManager()
{

	//Use clear and swap to ensure free memory
	this->gameObjectDefinitions.clear();
	map<string, GameObjectDefinition*>().swap(this->gameObjectDefinitions);
}


bool GameObjectManager::init()
{
	load("gameObjectDefinitions/commonObjects");
	load("gameObjectDefinitions/particleObjects");
	load("gameObjectDefinitions/level1Objects");
	load("gameObjectDefinitions/GUIObjects");
	load("gameObjectDefinitions/compositeObjects");

	return true;
}


void GameObjectManager::load(string gameObjectAssetsFilename)
{
	//Read file and stream it to a JSON object
	Json::Value root;
	string filename = "assets/" + gameObjectAssetsFilename + ".json";
	ifstream ifs(filename);
	ifs >> root;

	string id, textureId;
	GameObjectDefinition* gameObjectDefinition;

	for (auto itr : root["gameObjects"])
	{
		gameObjectDefinition = new GameObjectDefinition();
		gameObjectDefinition->id = itr["id"].asString();
		gameObjectDefinition->description = itr["description"].asString();
		gameObjectDefinition->speed = itr["speed"].asDouble();
		gameObjectDefinition->xSize = itr["xSize"].asFloat();
		gameObjectDefinition->ySize = itr["ySize"].asFloat();
		gameObjectDefinition->textureId = itr["texture"].asString();
		gameObjectDefinition->absolutePositioning = itr["absolutePositioning"].asBool();
		gameObjectDefinition->renderOutline = itr["renderOutline"].asBool();

		gameObjectDefinition->xRenderAdjustment = itr["xRenderAdjustment"].asFloat();
		gameObjectDefinition->yRenderAdjustment = itr["yRenderAdjustment"].asFloat();
		gameObjectDefinition->fireOffset = itr["fireOffset"].asFloat();


		//Game Object Type
		gameObjectDefinition->type = itr["type"].asString();

		//color
		if (itr["color"].isNull() == false)
		{
			if (itr["color"]["random"].isNull() == false &&
				itr["color"]["random"].asBool() == true)
			{
					gameObjectDefinition->color = util::generateRandomColor();
			}
			else
			{
				gameObjectDefinition->color.r = itr["color"]["red"].asInt();
				gameObjectDefinition->color.g = itr["color"]["green"].asInt();
				gameObjectDefinition->color.b = itr["color"]["blue"].asInt();
				gameObjectDefinition->color.a = itr["color"]["alpha"].asInt();
			}
		}
		else //default to white
		{
			gameObjectDefinition->color.r = 255;
			gameObjectDefinition->color.g = 255;
			gameObjectDefinition->color.b = 255;
			gameObjectDefinition->color.a = 255;
		}


		//If this is a physics object then build the box2d body
		if (itr["physicsObject"].isNull() == false)
		{
			gameObjectDefinition->isPhysicsObject = true;
			gameObjectDefinition->friction = itr["physicsObject"]["friction"].asFloat();
			gameObjectDefinition->restitution = itr["physicsObject"]["restitution"].asFloat();
			gameObjectDefinition->density = itr["physicsObject"]["density"].asFloat();
			gameObjectDefinition->linearDamping = itr["physicsObject"]["linearDamping"].asFloat();
			gameObjectDefinition->angularDamping = itr["physicsObject"]["angularDamping"].asFloat();
			gameObjectDefinition->physicsType = itr["physicsObject"]["type"].asString();
			gameObjectDefinition->collisionShape = itr["physicsObject"]["collisionShape"].asString();
			gameObjectDefinition->collisionRadius = itr["physicsObject"]["collisionRadius"].asFloat();
			gameObjectDefinition->collisionCategory = itr["physicsObject"]["collisionCategory"].asInt();
			gameObjectDefinition->weaponAnchorPoint.x = itr["physicsObject"]["weaponAnchorPoint"]["x"].asFloat();
			gameObjectDefinition->weaponAnchorPoint.y = itr["physicsObject"]["weaponAnchorPoint"]["y"].asFloat();

		}

		//If this object is a selectable or clickable object
		if (itr["uiEvents"].isNull() == false)
		{
			gameObjectDefinition->isMouseSelectable = true;
			gameObjectDefinition->onClickAction = itr["uiEvents"]["onClickAction"].asString();

		}
		//If this is a text object then store text details
		if (itr["text"].isNull() == false)
		{
			gameObjectDefinition->isTextObject = true;
			gameObjectDefinition->textDetails.value = itr["text"]["value"].asString();
			gameObjectDefinition->textDetails.fontId = itr["text"]["font"].asString();
			gameObjectDefinition->textDetails.isDynamic = itr["text"]["dynamic"].asBool();
			gameObjectDefinition->textDetails.size = itr["text"]["size"].asInt();
			if (itr["text"]["color"].isNull() == false)
			{
				gameObjectDefinition->textDetails.color.a = itr["text"]["color"]["alpha"].asInt();
				gameObjectDefinition->textDetails.color.r = itr["text"]["color"]["red"].asInt();
				gameObjectDefinition->textDetails.color.g = itr["text"]["color"]["green"].asInt();
				gameObjectDefinition->textDetails.color.b = itr["text"]["color"]["blue"].asInt();
			}
			else
			{
				gameObjectDefinition->textDetails.color.a = 255;
				gameObjectDefinition->textDetails.color.r = 255;
				gameObjectDefinition->textDetails.color.g = 255;
				gameObjectDefinition->textDetails.color.b = 255;

			}

		}

		//Store Animations
		if (itr["animations"].isNull() == false)
		{
			gameObjectDefinition->isAnimated = true;
			
			for (auto animItr : itr["animations"])
			{
				AnimationItem animation;
				animation.state = animItr["state"].asString();
				animation.textureId = animItr["textureId"].asString();
				animation.speed = animItr["speed"].asFloat();
				animation.frames = animItr["frames"].asInt();
				//animation = new Animation(gameObjectDefinition, id, texture, frames, speed);
				gameObjectDefinition->animationDetails.animations.push_back(animation);
			}
		}

		//Child Objects
		gameObjectDefinition->childPadding = itr["childPadding"].asFloat();
		gameObjectDefinition->childPositionRelative = itr["childPositionRelative"].asBool();
		if (itr["childObjects"].isNull() == false)
		{
			gameObjectDefinition->hasChildObjects = true;
			ChildObjectDetails childDetails;
			for (auto childItr : itr["childObjects"])
			{
				childDetails.gameObjectId = childItr["gameObjectId"].asString();
				childDetails.locationSlot = childItr["locationSlot"].asInt();
				childDetails.gameObjectType = childItr["gameObjectType"].asString();
				gameObjectDefinition->childObjectDefinitions.push_back(childDetails);
			}
		}

		//CompositeObject Details
		if (itr["composite"].isNull() == false)
		{
			gameObjectDefinition->isCompositeObject = true;
			gameObjectDefinition->compositeDetails.levelUpSpeed = itr["composite"]["levelUpSpeed"].asFloat();

			//Composite bluprint textureId
			gameObjectDefinition->compositeDetails.blueprint.textureId =
				itr["composite"]["blueprint"]["texture"].asString();

			//loop though all legend items that define how the composite should be built
			CompositeLegendItem legendItem;
			for (auto legendItr : itr["composite"]["blueprint"]["legend"])
			{
				//GameObjectId
				legendItem.gameObjectId = legendItr["gameObjectId"].asString();

				//color
				legendItem.color.r = legendItr["color"]["red"].asInt();
				legendItem.color.g = legendItr["color"]["green"].asInt();
				legendItem.color.b = legendItr["color"]["blue"].asInt();
				legendItem.color.a = legendItr["color"]["alpha"].asInt();

				gameObjectDefinition->compositeDetails.blueprint.legend.push_back(legendItem);

			}

			//CompositeLegendItem legendItem;
			CompositeLevel compositeLevel;
			for (auto levelItr : itr["composite"]["levels"])
			{
				compositeLevel.levelNum = levelItr["level"].asInt();
				compositeLevel.strength = levelItr["strength"].asInt();

				compositeLevel.color.r = levelItr["color"]["red"].asInt();
				compositeLevel.color.g = levelItr["color"]["green"].asInt();
				compositeLevel.color.b = levelItr["color"]["blue"].asInt();
				compositeLevel.color.a = levelItr["color"]["alpha"].asInt();

				gameObjectDefinition->compositeDetails.levels.push_back(compositeLevel);

			}
		}

		/*
		Weapons details
		*/
		float x = itr["physicsObject"]["anchorPoint"]["x"].asFloat();
		float y = itr["physicsObject"]["anchorPoint"]["y"].asFloat();
		gameObjectDefinition->weaponDetails.anchorPoint.Set(x,y);
		WeaponLevelDetails weaponLevelDetails;
		for (auto weaponItr : itr["weaponLevels"])
		{
			weaponLevelDetails.level = weaponItr["level"].asInt();
			weaponLevelDetails.levelUpTarget = weaponItr["levelUpTarget"].asInt();
			weaponLevelDetails.strength = weaponItr["strength"].asInt();
			weaponLevelDetails.bulletPoolId = weaponItr["bulletPoolId"].asString();
			weaponLevelDetails.color.r = weaponItr["color"]["red"].asInt();
			weaponLevelDetails.color.g = weaponItr["color"]["green"].asInt();
			weaponLevelDetails.color.b = weaponItr["color"]["blue"].asInt();
			weaponLevelDetails.color.a = weaponItr["color"]["alpha"].asInt();
			gameObjectDefinition->weaponDetails.weaponLevelDetails.emplace(weaponLevelDetails.level, weaponLevelDetails);

		}

		this->gameObjectDefinitions[gameObjectDefinition->id] = gameObjectDefinition;

	}
}



/*
	Retrieve the GameObjetc Definition
*/
GameObjectDefinition* GameObjectManager::getDefinition(string definitionId)
{

	if (this->gameObjectDefinitions.find(definitionId) == this->gameObjectDefinitions.end())
	{
		return NULL;
	}
	else
	{
		return this->gameObjectDefinitions[definitionId];
	}

}





