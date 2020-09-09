#include "ContinueGameClickAction.h"
#include "../SceneManager.h"

#include "../Scene.h"

ContinueGameClickAction::ContinueGameClickAction()
{

}

ContinueGameClickAction::~ContinueGameClickAction()
{

}

void ContinueGameClickAction::perform(GameObject* gameObject)
{

	SceneAction sceneAction = { SCENE_ACTION_EXIT ,"" };
	SceneManager::instance().setDirectSceneAction(sceneAction);

}
