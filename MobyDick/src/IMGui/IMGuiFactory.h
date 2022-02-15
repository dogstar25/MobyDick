#pragma once
#include <memory>
#include <string>
#include "IMGuiItem.h"

class IMGuiFactory
{
public:
	IMGuiFactory() = default;

	virtual std::shared_ptr<IMGuiItem> create(std::string);

private:

};
