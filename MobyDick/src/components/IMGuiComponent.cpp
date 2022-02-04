#include "IMGuiComponent.h"
#include "../IMGui/IMGuiSettingsMenu.h"


IMGuiComponent::IMGuiComponent(Json::Value componentJSON)
{
	auto imguiId = componentJSON["imguiId"].asString();
	create(imguiId);
}

void IMGuiComponent::create(std::string IMGuiId)
{

	if (IMGuiId == "IMGuiSettingsMenu"){

		m_IMGuiItem = std::make_shared<IMGuiSettingsMenu>();

	}


}


void IMGuiComponent::update()
{

}

void IMGuiComponent::render()
{

	std::dynamic_pointer_cast<IMGuiSettingsMenu>(m_IMGuiItem)->run();

}