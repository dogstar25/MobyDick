#pragma once
#include <stdio.h>
#include <string>
#include <map>
#include <memory>
#include <chrono>

using namespace std::chrono;
using namespace std;

struct textItem {

public:
	bool hasChanged;
	string text;

	steady_clock::time_point time_snapshot;
	//TODO: Add a timestamp variable to be checked during "if text changed" logic
	//		so that a new dynamic text texture is not generated more than once 
	//		per "second?". the refresh interval should be a config value of the 
	//		dynamic text manager

};


class DynamicTextManager
{
public:
	DynamicTextManager();
	~DynamicTextManager();

	map<string, unique_ptr<textItem>> textItems;

	//vector <unique_ptr<GameObject>> gameObjects;

	bool updateText(string, string);
	textItem* getTextItem(string);


};

