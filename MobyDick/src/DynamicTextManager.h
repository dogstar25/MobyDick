#ifndef DYNAMIC_TEXT_MANAGER_H
#define DYNAMIC_TEXT_MANAGER_H

#include <string>
#include <map>
#include <memory>
#include <chrono>

struct TextItem {

	bool hasChanged;
	std::string textValue;
	std::chrono::steady_clock::time_point time_snapshot;

};


class DynamicTextManager
{
public:

	static DynamicTextManager& instance();

	bool updateText(std::string, std::string);
	TextItem* getTextItem(std::string);

private:
	DynamicTextManager();
	~DynamicTextManager();

	std::map<std::string, std::unique_ptr<TextItem>> m_textItems;


};

#endif