#include "IMGuiTestLevelMenu.h"

#include "Scene.h"
#include <SDL2/SDL.h>
#include "MR_IMGuiUtil.h"
#include "IMGui/IMGuiUtil.h"

#include "../GameConstants.h"
#include "../MRContextManager.h"


extern std::unique_ptr<Game> game;


IMGuiTestLevelMenu::IMGuiTestLevelMenu(std::string gameObjectType, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor,
	ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize) :
	IMGuiItem(gameObjectType, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize)
{
	m_flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize;

}

glm::vec2 IMGuiTestLevelMenu::render()
{
	ImVec2 buttonSize{ ImGui::MRSettings::button1Size };
	glm::vec2 windowSize{};

	const auto& renderComponent = parent()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	setWindowProperties(parent());

	//Set Color
	ImGui::PushStyleColor(ImGuiCol_WindowBg, m_backgroundColor);

	ImGui::Begin(m_gameObjectType.c_str(), nullptr, m_flags);
	{

		ImGui::PushStyleColor(ImGuiCol_Button, m_buttonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_buttonHoverColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_buttonActiveColor);

		//top window spacing
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		//Menu Options
		ImGui::Text("LCTRL +");

		//"L" - Load Level
		ImGui::SmallButton("L");
		ImGui::SameLine();
		ImGui::Text("Load Level");
		if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_L)) {
			ImGui::OpenPopup("levelInput");
		}

		//"A" - Add Game Object
		ImGui::SmallButton("A");
		ImGui::SameLine();
		ImGui::Text("Add GameObject");

		//"D" - Delete Game Object
		ImGui::SmallButton("D");
		ImGui::SameLine();
		ImGui::Text("Delete GameObject");
		if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_D)) {
			ImGui::OpenPopup("deleteObject");
		}

		//ESC - Quit
		if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
			ImGui::OpenPopup("quit");
		}


		//Key handlers




		//Modal Handlers
		if (ImGui::BeginPopupModal("levelInput", nullptr)) {
			levelInput();
		}
		if (ImGui::BeginPopupModal("quit", nullptr)) {
			quit();
		}
		if (ImGui::BeginPopupModal("deleteObject", nullptr)) {
			deleteObject();
		}

		//spacing
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();


		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();


		windowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };

	}

	ImGui::End();

	ImGui::PopStyleColor();

	return windowSize;

}

std::string IMGuiTestLevelMenu::levelInput()
{

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	//ImGui::SetWindowPos(ImVec2{ 50,50});
	ImGui::SetWindowSize(ImVec2{ 250,150 });


	// Mouse sensitivity setting slider
	ImGui::SetKeyboardFocusHere();
	static char levelInput[128] = "1";
	ImGui::InputText("level", levelInput, IM_ARRAYSIZE(levelInput));

	if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {
		ImGui::CloseCurrentPopup();
		util::sendSceneEvent(SCENE_ACTION_LOAD_LEVEL, levelInput);
	}

	if (ImGui::IsKeyPressed(ImGuiKey_Escape) || ImGui::IsKeyPressed(ImGuiKey_RightCtrl)) {
		ImGui::CloseCurrentPopup();

	}

	ImGui::EndPopup();

	return std::string(levelInput);

}

void IMGuiTestLevelMenu::deleteObject()
{

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	//ImGui::SetWindowPos(ImVec2{ 50,50});
	ImGui::SetWindowSize(ImVec2{ 250,150 });


	// Mouse sensitivity setting slider
	ImGui::SetKeyboardFocusHere();
	static char objectName[128] = "1";
	ImGui::InputText("name", objectName, IM_ARRAYSIZE(objectName));

	if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {

		ImGui::CloseCurrentPopup();
		auto& scene = SceneManager::instance().getScene("SCENE_TEST");
		auto gameObject = scene.getGameObjectByName(objectName);
		if (gameObject.has_value()) {
			gameObject.value()->setRemoveFromWorld(true);
		}
	}

	if (ImGui::IsKeyPressed(ImGuiKey_Escape) || ImGui::IsKeyPressed(ImGuiKey_RightCtrl)) {
		ImGui::CloseCurrentPopup();

	}

	ImGui::EndPopup();

	return;

}

void IMGuiTestLevelMenu::quit()
{

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	//ImGui::SetWindowPos(ImVec2{ 50,50});
	ImGui::SetWindowSize(ImVec2{ 250,150 });
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::SetKeyboardFocusHere();
	ImGui::Text("Quit?");

	//"Y" - Yes Quit
	ImGui::SmallButton("Y");
	ImGui::SameLine();
	ImGui::Text("Yes");

	//"N" - No Quit
	ImGui::SmallButton("N");
	ImGui::SameLine();
	ImGui::Text("No");

	if (ImGui::IsKeyPressed(ImGuiKey_Y)) {
		ImGui::CloseCurrentPopup();
		util::sendSceneEvent(SCENE_ACTION_REPLACE, "SCENE_TITLE_SCREEN");
	}
	else if (ImGui::IsKeyPressed(ImGuiKey_N) ) {
		ImGui::CloseCurrentPopup();
	}

	
	//if (ImGui::IsKeyPressed(ImGuiKey_Escape) || ImGui::IsKeyPressed(ImGuiKey_RightCtrl)) {
	//	ImGui::CloseCurrentPopup();

	//}

	ImGui::EndPopup();

	return;
}


