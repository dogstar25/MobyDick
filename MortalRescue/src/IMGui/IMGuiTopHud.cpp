#include "IMGuiTopHud.h"

#include "ContextManager.h"
#include "Util.h"
#include "game.h"
#include <memory>
#include "../GameConstants.h"

extern std::unique_ptr<Game> game;

IMGuiTopHud::IMGuiTopHud(std::string gameObjectId, b2Vec2 padding, ImVec4 backgroundColor, ImVec4 textColor,
	ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize) :
	IMGuiItem(gameObjectId, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize)
{

	m_flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	//Define some colors for HuD Elements
	SDL_Color red = Colors::RED;
	util::colorApplyAlpha(red, 255);
	m_hudRed = util::SDLColorToImVec4(red);

	SDL_Color white = Colors::WHITE;
	util::colorApplyAlpha(white, 255);
	m_hudWhite = util::SDLColorToImVec4(white);



}

glm::vec2 IMGuiTopHud::render()
{


	glm::vec2 windowSize{};

	const auto& renderComponent = parent()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	setWindowProperties(parent());

	//Set Color
	
	ImGui::PushStyleColor(ImGuiCol_WindowBg, m_backgroundColor);

	ImGui::Begin(m_gameObjectId.c_str(), nullptr, m_flags);
	{

		hudLives();
		hudScrapCount();
		hudScrapBar();

		windowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };
		
	}
	ImGui::End();

	//pop color style
	ImGui::PopStyleColor();

	return windowSize;


}

void IMGuiTopHud::hudLives()
{
	glm::vec2 topLeft{};
	glm::vec2 bottomRight{};

	//TextureAtlas Coordinates
	glm::vec2 topLeftFull = util::glNormalizeTextureCoords({ 65,33 }, { 256, 256 });
	glm::vec2 bottomRightFull = util::glNormalizeTextureCoords({ 128,98 }, { 256, 256 });

	glm::vec2 topLeftDead = util::glNormalizeTextureCoords({ 130,33 }, { 256, 256 });
	glm::vec2 bottomRightDead = util::glNormalizeTextureCoords({ 193,98 }, { 256, 256 });

	auto& livesStatusItem = game->contextMananger()->getStatusItem("LIVES_COUNT");

	for (int life = 0; life < livesStatusItem.maxValue(); life++) {


		if (life < livesStatusItem.value()) {
			topLeft = topLeftFull;
			bottomRight = bottomRightFull;
		}
		else {
			topLeft = topLeftDead;
			bottomRight = bottomRightDead;

		}

		if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

			GLuint textureAtlasId = static_cast<GLRenderer*>(game->renderer())->getTextureId(GL_TextureIndexType::IMGUI_TEXTURE_ATLAS);
			ImGui::Image((void*)(int*)textureAtlasId, ImVec2(32, 32), ImVec2(bottomRight.x, bottomRight.y), ImVec2(topLeft.x, topLeft.y), m_hudRed);
			ImGui::SameLine(0.0f, 2);
		}
		else {

			//SDL2 Texture void* is the SDL_Texture*
			SDL_Texture* sdlTexture = TextureManager::instance().getTexture("TEXTURE_IMGUI_ATLAS")->sdlTexture;
			ImGui::Image((void*)(SDL_Texture*)sdlTexture, ImVec2(32, 32), ImVec2(topLeft.x, topLeft.y), ImVec2(bottomRight.x, bottomRight.y), m_hudRed);

			ImGui::SameLine(0.0f, 2);
		}

	}

	ImGui::NewLine();

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

	//TextureAtlas Coordinates
	glm::vec2 topLeft = util::glNormalizeTextureCoords({ 0,0 }, { 256, 256 });
	glm::vec2 bottomRight = util::glNormalizeTextureCoords({ 4,16 }, { 256, 256 });

	for (int i = 0; i < game->contextMananger()->getValue("SCRAP_COUNT"); i++) {

		if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

			GLuint textureAtlasId = static_cast<GLRenderer*>(game->renderer())->getTextureId(GL_TextureIndexType::IMGUI_TEXTURE_ATLAS);
			ImGui::Image((void*)(int*)textureAtlasId, ImVec2(2, 32), ImVec2(bottomRight.x, bottomRight.y), ImVec2(topLeft.x, topLeft.y), m_hudRed);
			ImGui::SameLine(0.0f, 0);
		}
		else {

			//SDL2 Texture void* is the SDL_Texture*
			SDL_Texture* sdlTexture = TextureManager::instance().getTexture("TEXTURE_IMGUI_ATLAS")->sdlTexture;
			ImGui::Image((void*)(SDL_Texture*)sdlTexture, ImVec2(2, 32), ImVec2(topLeft.x, topLeft.y), ImVec2(bottomRight.x, bottomRight.y), m_hudRed);
			ImGui::SameLine(0.0f, 0);
		}

	}

}

