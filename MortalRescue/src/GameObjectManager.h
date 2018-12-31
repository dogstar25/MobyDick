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
using namespace std;

class GameObjectManager
{
public:
	
	bool init(TextureManager*, b2World*);
	void testBlocks(SDL_Event*, b2World*);


	GameObject* getGameObject(string id);
	map<string, GameObject> gameObjectMap;

private:
	b2Body* buildB2Body(GameObject*, b2World*);
};

