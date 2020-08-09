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
	m_MoveActionMap["ActorMove"] = std::make_shared<ActorMoveAction>();
	m_RotateActionMap["ActorRotate"] = std::make_shared<ActorRotateAction>();
	m_UseActionMap["ActorUse"] = std::make_shared<ActorUseAction>();
	m_onHoverActionMap["DefaultHover"] = std::make_shared<DefaultOnHoverAction>();
	m_onHoverActionMap["ContinueClick"] = std::make_shared<ContinueGameClickAction>();


}


ActionMaps::~ActionMaps()
{

}