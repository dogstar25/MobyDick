#pragma once

#include <map>
#include <string>
#include "HeroFireAction.h"



class Action
{

	void init();

	template<typename T> Action* createInstance() { return new T; }

	typedef std::map<std::string, Action* (*)()> map_type;

	map_type map;
	




};

