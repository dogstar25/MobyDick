#pragma once
#include <memory>
#include <string>
#include <json/json.h>

class IMGuiItem;

class IMGuiFactory
{
public:
	IMGuiFactory() = default;

	virtual std::shared_ptr<IMGuiItem> create(std::string, std::string windowName, Json::Value parms);

private:

};
