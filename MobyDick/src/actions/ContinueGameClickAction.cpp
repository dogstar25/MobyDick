#include "ContinueGameClickAction.h"


#include "../Scene.h"

ContinueGameClickAction::ContinueGameClickAction()
{

}

ContinueGameClickAction::~ContinueGameClickAction()
{

}

void ContinueGameClickAction::perform(GameObject* gameObject)
{

	SDL_Event event;

	SceneAction* sceneAction = new SceneAction();
	sceneAction->actionCode = SCENE_ACTION_EXIT;
	sceneAction->sceneId = "";

	event.type = SDL_USEREVENT;
	event.user.data1 = sceneAction;
	SDL_PushEvent(&event);

}
