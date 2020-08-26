#pragma once
#include <string>
#include <array>
#include <vector>
#pragma warning(push,0)
#include <box2d/box2d.h>
#pragma warning(pop)

#include "../Globals.h"

using namespace std;

namespace SceneConfigData {

	struct location { int x; int y; };

	struct gameObject
	{
		string			gameObjectId;
		location		location;
	};

	struct scene
	{
		string				id;
		size_t				mouseMode;
		int					maxGameObjects;
		vector<string>		levels;
		vector<gameObject>	gameObjects;
	};

	vector<scene> scenes =
	{
		{
			"SCENE_PLAY", 
			CONTROL_MODE_PLAY,	
			2000,	

			{
				"level1", "level2", "level3"
			},
			{							
				{ "gina", 
					{2,2} 
				}, 	
				{ "gina", 
					{2,2} 
				} 
			}
		}
	};

}