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
	GameObjectManager();
	~GameObjectManager();

	bool init(TextureManager*);
	GameObject* getGameObject(string id);

private:
	map<string, GameObject> gameObjectMap;
};

