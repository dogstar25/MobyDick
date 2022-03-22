#include "ContinueGameClickAction.h"


#include "../Scene.h"

void ContinueGameClickAction::perform()
{

	SDL_Event event;

	SceneAction* sceneAction = new SceneAction();
	sceneAction->actionCode = SCENE_ACTION_EXIT;
	sceneAction->actionId = "";

	event.type = SDL_USEREVENT;
	event.user.data1 = sceneAction;
	SDL_PushEvent(&event);

}
