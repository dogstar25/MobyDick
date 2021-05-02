#pragma once
#include "Action.h"
class LoadNextLevelAction :  public Action
{

public:

	LoadNextLevelAction();
	~LoadNextLevelAction();
	LoadNextLevelAction(std::tuple<std::string, int, float> params) :
		m_params(params) {}

	void setParams(std::tuple<std::string, int, float> params) {
		m_params = params;
	}

	void perform() override;

protected:
	std::tuple<std::string, int, float> m_params;
};

