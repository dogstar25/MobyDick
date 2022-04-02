#include "IMGuiTopHud.h"

#include "ContextManager.h"
#include "Util.h"
#include "game.h"
#include <memory>
#include "../GameConstants.h"

extern std::unique_ptr<Game> game;

IMGuiTopHud::IMGuiTopHud(std::string gameObjectId, b2Vec2 padding, ImVec4 color, bool autoSize) :
	IMGuiItem(gameObjectId, padding, color, autoSize)
{

	m_flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

}

glm::vec2 IMGuiTopHud::render(GameObject* parentGameObject)
{


	glm::vec2 windowSize{};

	const auto& renderComponent = parentGameObject->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	setWindowProperties(parentGameObject);

	//Set Color
	ImGui::PushStyleColor(ImGuiCol_WindowBg, m_color);

	ImGui::Begin(m_gameObjectId.c_str(), nullptr, m_flags);
	{

		hudScrapCount();
		hudScrapBar();

		windowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };
		
	}
	ImGui::End();

	//pop color style
	ImGui::PopStyleColor();

	return windowSize;


}

void IMGuiTopHud::hudScrapCount()
{

	ImGui::Text("%s  %d", "ScrapCount", (int)game->contextMananger()->getValue("SCRAP_COUNT"));

}

void IMGuiTopHud::hudScrapBar()
{
	//CurrLineTextBaseOffset
	ImGui::Text("Scrap");
	ImGui::SameLine();
	//ImGui::BeginGroup();
	ImVec4 red1 = { 1.0, 0.0, 0.0, 0.78 };
	ImGui::PushStyleColor(ImGuiCol_WindowBg, red1);

	//Calculate the texture Atlas source rectangle (normalized)
	glm::vec2 topLeft = util::glNormalizeTextureCoords({ 37,92 }, { 4096, 4096 });
	glm::vec2 bottomRight = util::glNormalizeTextureCoords({ 68,123 }, { 4096, 4096 });

	for (int i = 0; i < game->contextMananger()->getValue("SCRAP_COUNT"); i++) {

		if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

			GLuint textureAtlasId = static_cast<GLRenderer*>(game->renderer())->getTextureId(GL_TextureIndexType::MAIN_TEXTURE_ATLAS);
			ImGui::Image((void*)(int*)textureAtlasId, ImVec2(2, 32), ImVec2(.00122, .000122), ImVec2(.00244, .00244));
		}
		else {

			//SDL2 Texture void* is the SDL_Texture*
			SDL_Texture* sdlTexture = TextureManager::instance().getTexture("TEXTURE_ATLAS_0")->sdlTexture;
			ImGui::Image((void*)(SDL_Texture*)sdlTexture, ImVec2(2, 32), ImVec2(topLeft.x, topLeft.y), ImVec2(bottomRight.x, bottomRight.y), red1);
			ImGui::SameLine(0.0f, 0);
		}

	}

	if (ImGui::IsKeyPressed(ImGuiKey_G)) {
		std::cout << "Top Hud Key Pressed" << "\n";
	}



	ImGui::PopStyleColor();
}

