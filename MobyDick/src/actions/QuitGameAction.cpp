#include "QuitGameAction.h"
#include "../SceneManager.h"


#include "../Scene.h"

QuitGameAction::QuitGameAction()
{

}

QuitGameAction::~QuitGameAction()
{

}

void QuitGameAction::perform(GameObject* gameObject)
{

	SceneAction sceneAction = { SCENE_ACTION_QUIT ,"" };
	SceneManager::instance().setDirectSceneAction(sceneAction);


}
