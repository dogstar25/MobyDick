#include "IMGuiTopHud.h"

#include "ContextManager.h"
#include "Util.h"
#include "game.h"
#include <memory>
#include "../GameConstants.h"
#include "imgui/IMGuiUtil.h"


extern std::unique_ptr<Game> game;

IMGuiTopHud::IMGuiTopHud(std::string gameObjectType, b2Vec2 padding, Scene* parentScene, ImVec4 backgroundColor, ImVec4 textColor,
	ImVec4 buttonColor, ImVec4 buttonHoverColor, ImVec4 buttonActiveColor, bool autoSize) :
	IMGuiItem(gameObjectType, padding, backgroundColor, textColor, buttonColor, buttonHoverColor, buttonActiveColor, autoSize)
{

	m_flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	//Define some colors for HuD Elements
	SDL_Color red = Colors::RED;
	util::colorApplyAlpha(red, 255);
	m_hudRed = util::SDLColorToImVec4(red);

	SDL_Color white = Colors::WHITE;
	util::colorApplyAlpha(white, 255);
	m_hudWhite = util::SDLColorToImVec4(white);

	SDL_Color green = Colors::EMERALD;
	util::colorApplyAlpha(green, 255);
	m_hudGreen = util::SDLColorToImVec4(green);

	SDL_Color blue = Colors::BLUE;
	util::colorApplyAlpha(blue, 255);
	m_hudBlue = util::SDLColorToImVec4(blue);


	//testObject = std::make_shared<GameObject>("SWORDLADY", -1.0F, -1.0F, 0.F, parentScene);


}

glm::vec2 IMGuiTopHud::render()
{


	glm::vec2 windowSize{};

	const auto& renderComponent = parent()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	setWindowProperties(parent());

	//Set Color
	
	ImGui::PushStyleColor(ImGuiCol_WindowBg, m_backgroundColor);

	ImGui::Begin(m_gameObjectType.c_str(), nullptr, m_flags);
	{

		hudLevel();

		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

		hudLives();

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
	ImVec4 heartColor{};

	//TextureAtlas Coordinates
	glm::vec2 topLeftFull = util::glNormalizeTextureCoords({ 65,33 }, { 256, 256 });
	glm::vec2 bottomRightFull = util::glNormalizeTextureCoords({ 128,98 }, { 256, 256 });

	glm::vec2 topLeftDead = util::glNormalizeTextureCoords({ 130,33 }, { 256, 256 });
	glm::vec2 bottomRightDead = util::glNormalizeTextureCoords({ 193,98 }, { 256, 256 });

	auto& livesStatusItem = game->contextMananger()->getStatusItem(StatusItemId::PLAYERS_HEART_COUNT);

	for (int life = 0; life < livesStatusItem.maxValue(); life++) {


		if (life < livesStatusItem.value()) {
			topLeft = topLeftFull;
			bottomRight = bottomRightFull;
			heartColor = m_hudRed;
		}
		else {
			topLeft = topLeftDead;
			bottomRight = bottomRightDead;
			heartColor = m_hudRed;
		}

		if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

			GLuint textureAtlasId = static_cast<GLRenderer*>(game->renderer())->getTextureId(GL_TextureIndexType::IMGUI_TEXTURE_ATLAS);
			ImGui::Image((void*)(int*)textureAtlasId, ImVec2(32, 32), ImVec2(topLeft.x, topLeft.y), ImVec2(bottomRight.x, bottomRight.y), heartColor);
			ImGui::SameLine(0.0f, 2);
		}
		else {

			//SDL2 Texture void* is the SDL_Texture*
			SDL_Texture* sdlTexture = TextureManager::instance().getTexture("TEXTURE_IMGUI_ATLAS")->sdlTexture;
			ImGui::Image((void*)(SDL_Texture*)sdlTexture, ImVec2(32, 32), ImVec2(topLeft.x, topLeft.y), ImVec2(bottomRight.x, bottomRight.y), heartColor);

			ImGui::SameLine(0.0f, 2);
		}

	}

}

void IMGuiTopHud::hudLevel()
{
	//Get the value for the current player weapon levelup accrual
	auto& level = game->contextMananger()->getStatusItem(StatusItemId::CURRENT_LEVEL);

	//Level Text
	std::stringstream levelTxtSS;
	levelTxtSS << "Level " << level.value();

	ImGui::PushFont(m_mediumFont);
	ImGui::textCentered(levelTxtSS.str());
	ImGui::PopFont();



}


