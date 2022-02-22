#include "IMGuiItem.h"
#include "../game.h"

extern std::unique_ptr<Game> game;

IMGuiItem::IMGuiItem(Json::Value paramsJSON, std::string gameObjectName)
{
	
	m_params = paramsJSON;
	m_name = gameObjectName;





}

//glm::vec2 IMGuiItem::size()
//{
//
//	ImGuiStyle& style = ImGui::GetStyle();
//	style.WindowMinSize = ImVec2{ 12,12 };
//	style.WindowPadding = ImVec2{ 8,8 };
//	style.WindowBorderSize = 0.;
//	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
//
//	ImGuiWindowFlags flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;
//	flags |= ImGuiWindowFlags_AlwaysAutoResize;
//
//	ImGui::Begin(m_name.c_str(), nullptr, flags);
//		auto size = ImGui::GetWindowSize();
//		ImGui::PopStyleColor();
//	ImGui::End();
//
//	return (glm::vec2{size.x, size.y});
//}
