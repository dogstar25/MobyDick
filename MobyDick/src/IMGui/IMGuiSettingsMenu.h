#pragma once
#include "IMGuiItem.h"

#include <memory>

class IMGuiSettingsMenu : public IMGuiItem
{
public:
	IMGuiSettingsMenu();
	void run() override;
	void sendSceneCloseEvent();

private:
	bool show_another_window = true;

};