#include "game.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "WorldObject.h"
#include "GameObjectDefinition.h"

#include <SDL.h>
#include <SDL_image.h>
#include <json/json.h>

#include <iostream>
#include <fstream>


GameObjectManager::GameObjectManager()
{

}

GameObjectManager::~GameObjectManager()
{

	//clean and delete all of the game objects
	/*
	for (auto gameObjectDefinition : this->gameObjectDefinitions) {

		delete gameObjectDefinition.second;
	}
	*/
	this->gameObjectDefinitions.clear();
}

bool GameObjectManager::init()
{
	load("gameObjects_Common");
	load("particleObjects");
	load("gameObjects_Level1");
	load("GUIObjects");

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

		//If this is a primitive, then it has no texture and needs a color
		if (itr["primative"].isNull() == false)
		{
			gameObjectDefinition->isPrimitive = true;

			//color
			if (itr["color"]["random"].asBool() == true)
			{
				gameObjectDefinition->color = game->util.generateRandomColor();
			}
			else
			{
				gameObjectDefinition->color.r = itr["primative"]["color"]["red"].asInt();
				gameObjectDefinition->color.g = itr["primative"]["color"]["green"].asInt();
				gameObjectDefinition->color.b = itr["primative"]["color"]["blue"].asInt();
				gameObjectDefinition->color.a = itr["primative"]["color"]["alpha"].asInt();
			}
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
			gameObjectDefinition->collisionGroup = itr["physicsObject"]["collisionGroup"].asInt();


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

		//If this is a GUI object then store text details
		if (itr["GUI"].isNull() == false)
		{
			gameObjectDefinition->textDetails.value = itr["text"]["value"].asString();
			gameObjectDefinition->textDetails.fontId = itr["text"]["font"].asString();
			if (itr["text"]["color"].isNull() == false)
			{
				gameObjectDefinition->textDetails.color.a = itr["text"]["color"]["alpha"].asInt();
				gameObjectDefinition->textDetails.color.r = itr["text"]["color"]["red"].asInt();
				gameObjectDefinition->textDetails.color.g = itr["text"]["color"]["green"].asInt();
				gameObjectDefinition->textDetails.color.b = itr["text"]["color"]["blue"].asInt();
			}
			else // default to grey
			{
				gameObjectDefinition->textDetails.color.a = 206;
				gameObjectDefinition->textDetails.color.r = 205;
				gameObjectDefinition->textDetails.color.g = 211;
				gameObjectDefinition->textDetails.color.b = 255;

			}

		}

		//Store Animations
		if (itr["animations"].isNull() == false)
		{
			gameObjectDefinition->isAnimated = true;
			Animation* animation;
			for (auto animItr : itr["animations"])
			{
				string texture = animItr["texture"].asString();
				string id = animItr["id"].asString();
				int frames = animItr["frames"].asInt();
				float speed = animItr["speed"].asFloat();
				animation = buildAnimation(gameObjectDefinition, id, texture, frames, speed);
				gameObjectDefinition->animations[id] = animation;
			}
		}

		this->gameObjectDefinitions[gameObjectDefinition->id] = gameObjectDefinition;

	}
}

/*
Build Animation object
*/
Animation* GameObjectManager::buildAnimation(GameObjectDefinition* gameObjectDefinition,
	string id, string textureId, int frames,
	float speed)
{

	Animation* animation = nullptr;
	animation = new Animation();

	animation->id = id;
	animation->frameCount = frames;
	animation->speed = speed;

	//Get pointer to textture
	animation->texture = game->textureManager.getTexture(textureId)->sdlTexture;

	//Calculate how many columns and rows this animation texture has
	int width, height;
	//First get width of textture
	SDL_QueryTexture(animation->texture, NULL, NULL, &width, &height);

	//Calculate nnumber of rows and columns - remember to convert the gameObject size to pixels first
	int rows, columns;
	if (gameObjectDefinition->isPhysicsObject == true)
	{
		columns = width / (gameObjectDefinition->xSize * game->config.scaleFactor);
		rows = height / (gameObjectDefinition->ySize * game->config.scaleFactor);
	}
	else
	{
		columns = width / (gameObjectDefinition->xSize);
		rows = height / (gameObjectDefinition->ySize);

	}

	//Calculate top left corner of each animation frame
	SDL_Point point;
	int frameCount = 0;
	for (int rowIdx = 0; rowIdx < rows; rowIdx++) {
		for (int colIdx = 0; colIdx < columns; colIdx++) {

			if (gameObjectDefinition->isPhysicsObject == true)
			{
				point.x = colIdx * (gameObjectDefinition->xSize * game->config.scaleFactor);
				point.y = rowIdx * (gameObjectDefinition->ySize * game->config.scaleFactor);
			}
			else
			{
				point.x = colIdx * (gameObjectDefinition->xSize);
				point.y = rowIdx * (gameObjectDefinition->ySize);

			}
			//animation->animationFramePositions[frameCount] = point;
			animation->animationFramePositions.push_back(point);

			//do not exceed the maximum number of frames that this texture holds
			frameCount++;
			if (frameCount >= animation->frameCount) {
				break;
			}
		}
	}

	//TODO: Initialze the current source rect to the first animation frame
	//SDL_Rect* sourceRect = nullptr;
	//animation->currentTextureAnimationSrcRect

	return animation;



}

/*
	Retrieve the GameObjetc Definition
*/
GameObjectDefinition* GameObjectManager::getDefinition(string definitionId)
{

	//TODO: return game object definition

	return this->gameObjectDefinitions[definitionId];


}


