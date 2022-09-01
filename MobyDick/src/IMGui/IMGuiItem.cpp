#include "IMGuiItem.h"
#include "../game.h"

extern std::unique_ptr<Game> game;

IMGuiItem::IMGuiItem(std::string gameObjectType, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor, ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize)
{
	
	ImGuiIO& io = ImGui::GetIO();


	m_gameObjectType = gameObjectType;
	m_autoSize = autoSize;
	m_padding = padding;
	m_backgroundColor = backgroundColor;
	m_textColor = textColor;
	m_buttonColor = buttonColor;
	m_buttonHoverColor = buttonHoverColor;
	m_buttonActiveColor = buttonActiveColor;

	//Fonts - these are static so only initialize if they are null
	//First font loaded into ImGui defaults to the default font
	if (!m_normalFont) {
		m_normalFont = io.Fonts->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 24.0f);
	}

	if (!m_xSmallFont) {
		m_xSmallFont = io.Fonts->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 8.0f);
	}
	if (!m_smallFont) {
		m_smallFont = io.Fonts->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 16.0f);
	}
	if (!m_mediumFont) {
		m_mediumFont = io.Fonts->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 32.0f);
	}
	if (!m_largeFont) {
		m_largeFont = io.Fonts->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 48.0f);
	}
	if (!m_xLargeFont) {
		m_xLargeFont = io.Fonts->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 64.0f);
	}

}

void IMGuiItem::setWindowProperties(GameObject* parentGameObject)
{
	const auto& renderComponent = parentGameObject->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	ImGuiStyle& style = ImGui::GetStyle();

	//If this is autosized, then set the autosize flag and 
	if (m_autoSize == true) {

		m_flags |= ImGuiWindowFlags_AlwaysAutoResize;

		//We dont know the size of this window if this is the first time being displayed so Imgui has a flag for that so that the 
		//window doesnt initially appear to jump/jitter
		if (renderComponent->getRenderDestRect().w == 0) {
			ImGui::SetNextWindowPos(ImVec2{ renderComponent->getRenderDestRect().x, renderComponent->getRenderDestRect().y }, ImGuiCond_FirstUseEver);
		}
		else {
			ImGui::SetNextWindowPos(ImVec2{ renderComponent->getRenderDestRect().x, renderComponent->getRenderDestRect().y }, ImGuiCond_Appearing);
		}

	}
	//If this is NOT autosized, then set the size as defined by the component
	else {
		ImGui::SetNextWindowSize(ImVec2{ renderComponent->getRenderDestRect().w, renderComponent->getRenderDestRect().h });
		ImGui::SetNextWindowPos(ImVec2{ renderComponent->getRenderDestRect().x, renderComponent->getRenderDestRect().y });
	}

	//Set window padding
	style.WindowPadding = ImVec2{ m_padding.x, m_padding.y };


}

