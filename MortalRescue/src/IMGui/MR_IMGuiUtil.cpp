#include "MR_IMGuiUtil.h"
#include "IMGui/IMGuiUtil.h"
#include "GameConfig.h"
#include "../MRContextManager.h"

extern std::unique_ptr<Game> game;

namespace ImGui
{

	void continueGameLoad()
	{

		std::stringstream levelStr{};

		//Get the current level from the saveGame file
		GameSaveFileData gameSaveFileData{};
		game->contextMananger()->loadGame(&gameSaveFileData);
		levelStr << gameSaveFileData.level;

		//Send change to SCENE_PLAY Event
		util::sendSceneEvent(SCENE_ACTION_REPLACE, "SCENE_PLAY");

		//Send LOAD_LEVEL evnt using the currentt saved level
		util::sendSceneEvent(SCENE_ACTION_LOAD_LEVEL, levelStr.str());

	}

	void newGameLoad()
	{

		SDL_Event event;
		SceneAction* sceneAction;
		std::stringstream levelStr{};

		//if level zero was passed in then load the current level that is saved in the save file
		GameSaveFileData gameSaveFileData{};
		game->contextMananger()->loadGame(&gameSaveFileData);
		gameSaveFileData.level = 1;
		game->contextMananger()->saveGame(&gameSaveFileData);
		levelStr << 1;

		//Send change to Play Scene Event
		sceneAction = new SceneAction();
		sceneAction->actionCode = SCENE_ACTION_REPLACE;
		sceneAction->actionId = "SCENE_PLAY";

		event.type = SDL_USEREVENT;
		event.user.data1 = sceneAction;
		SDL_PushEvent(&event);

		//Send load current level evnt
		sceneAction = new SceneAction();
		sceneAction->actionCode = SCENE_ACTION_LOAD_LEVEL;
		sceneAction->actionId = levelStr.str();

		event.type = SDL_USEREVENT;
		event.user.data1 = sceneAction;
		SDL_PushEvent(&event);


	}

}
