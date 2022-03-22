#include "ActionSettingsClick.h"

#include "../Scene.h"

void ActionSettingsClick::perform(GameObject* gameObject)
{

	SDL_Event event;

	SceneAction* sceneAction = new SceneAction();
	sceneAction->actionCode = SCENE_ACTION_ADD;
	sceneAction->actionId = "SCENE_SETTINGS_MENU";

	event.type = SDL_USEREVENT;
	event.user.data1 = sceneAction;
	SDL_PushEvent(&event);

}