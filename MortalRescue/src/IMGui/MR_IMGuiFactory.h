#pragma once
#include <memory>
#include <string>
#include "IMGui/IMGuiItem.h"
#include "IMGui/IMGuiFactory.h"

class MR_IMGuiFactory : public IMGuiFactory
{
public:
	MR_IMGuiFactory();

	std::shared_ptr<IMGuiItem> create(std::string) override;

private:

};
