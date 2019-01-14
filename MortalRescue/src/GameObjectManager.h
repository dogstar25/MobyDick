#pragma once
#include <string>
#include "GameObject.h"
#include <SDL.h>
#include <SDL_image.h>
#include <json/json.h>
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include "TextureManager.h"
#include "GameObjectDefinition.h"

using namespace std;

class GameObjectManager
{
public:
	
	bool init();
	b2Body* buildB2Body(GameObjectDefinition*, b2World*);
	GameObjectAnimation * buildAnimation(GameObjectDefinition*, string, string, int, float);
	GameObject * buildGameObject(string , b2World* );
	GameObjectDefinition* getGameObjectDefinition(string );

	//Map of the definitions of all posible game objects in the game/level
	map<string, GameObjectDefinition> gameObjectDefinitions;

private:
	
};

