#include "ActionMaps.h"
#include "ActorMoveAction.h"
#include "ActorRotateAction.h"
#include "ActorUseAction.h"
#include "ContinueGameClickAction.h"
#include "DefaultOnHoverAction.h"

ActionMaps& ActionMaps::instance()
{
	static ActionMaps singletonInstance;
	return singletonInstance;
}


ActionMaps::ActionMaps()
{
	m_actionMap["ActorMove"] = std::make_shared<ActorMoveAction>();
	m_actionMap["ActorRotate"] = std::make_shared<ActorRotateAction>();
	m_actionMap["ActorUse"] = std::make_shared<ActorUseAction>();
	m_actionMap["DefaultHover"] = std::make_shared<DefaultOnHoverAction>();
	m_actionMap["ContinueClick"] = std::make_shared<ContinueGameClickAction>();


}


ActionMaps::~ActionMaps()
{

}