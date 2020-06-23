#ifndef DYNAMIC_TEXT_MANAGER_H
#define DYNAMIC_TEXT_MANAGER_H

#include <string>
#include <map>
#include <memory>
#include <chrono>

struct textItem {

public:
	bool hasChanged;
	std::string text;
	std::chrono::steady_clock::time_point time_snapshot;

};


class DynamicTextManager
{
public:

	static DynamicTextManager& instance();
	std::map<std::string, std::unique_ptr<textItem>> textItems;

	bool updateText(std::string, std::string);
	textItem* getTextItem(std::string);

private:
	DynamicTextManager();
	~DynamicTextManager();


};

#endif