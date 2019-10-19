#pragma once
#include <stdio.h>
#include <string>
#include <map>
#include <memory>

using namespace std;

struct textItem {

public:
	bool hasChanged;
	string text;

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

