#include "IMGuiTopRightHud.h"

#include "ContextManager.h"
#include "Util.h"
#include "game.h"
#include <memory>
#include "../GameConstants.h"
#include "imgui/IMGuiUtil.h"


extern std::unique_ptr<Game> game;

IMGuiTopRightHud::IMGuiTopRightHud(std::string gameObjectId, b2Vec2 padding, Scene* parentScene, ImVec4 backgroundColor, ImVec4 textColor,
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

	SDL_Color green = Colors::EMERALD;
	util::colorApplyAlpha(green, 255);
	m_hudGreen = util::SDLColorToImVec4(green);

	SDL_Color blue = Colors::BLUE;
	util::colorApplyAlpha(blue, 255);
	m_hudBlue = util::SDLColorToImVec4(blue);

	SDL_Color yellow = Colors::YELLOW;
	util::colorApplyAlpha(yellow, 255);
	m_hudYellow = util::SDLColorToImVec4(yellow);

	

}

glm::vec2 IMGuiTopRightHud::render()
{

	glm::vec2 windowSize{};

	const auto& renderComponent = parent()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	setWindowProperties(parent());

	//Set Color

	ImGui::PushStyleColor(ImGuiCol_WindowBg, m_backgroundColor);

	ImGui::Begin(m_gameObjectId.c_str(), nullptr, m_flags);
	{

		survivorCount();

		windowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };

	}
	ImGui::End();

	//pop color style
	ImGui::PopStyleColor();

	return windowSize;


}

void IMGuiTopRightHud::survivorCount()
{

	ImVec4 gunColor{};

	//Get the value for the current player weapon levelup accrual
	auto& survivorCount = game->contextMananger()->getStatusItem("PLAYER_WEAPON_LEVEL_ACCRUAL");

	//Survivor image
	hudSurvivorImage(m_hudYellow);

	auto survivorsSaved = game->contextMananger()->getStatusItem("SURVIVORS_SAVED").value();
	auto survivorsTotal = game->contextMananger()->getStatusItem("SURVIVORS_SAVED").maxValue();

	//Level Text
	std::stringstream levelTxtSS;
	std::string levelTxt;
	levelTxtSS << survivorsSaved << "/" << survivorsTotal;
	levelTxt = levelTxtSS.str();
	ImGui::SameLine();
	//ImGui::setFont64();
	ImGui::PushFont(game->renderer()->font64());

	//ImGui::Dummy(ImVec2(0.0f, 32.0f));
	ImGui::Text(levelTxt.c_str());
	ImGui::PopFont();
	ImGui::NewLine();

}

void IMGuiTopRightHud::hudSurvivorImage(ImVec4 color)
{

	//const auto& transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	//testObject->setPosition(transform->getTopLeftPosition());
	//testObject->render();

	//TextureAtlas Coordinates for bar
	glm::vec2 topLeft = util::glNormalizeTextureCoords({ 65,98 }, { 256, 256 });
	glm::vec2 bottomRight = util::glNormalizeTextureCoords({ 128,161 }, { 256, 256 });

	if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

		GLuint textureAtlasId = static_cast<GLRenderer*>(game->renderer())->getTextureId(GL_TextureIndexType::IMGUI_TEXTURE_ATLAS);
		ImGui::Image((void*)(int*)textureAtlasId, ImVec2(64, 64), ImVec2(topLeft.x, topLeft.y), ImVec2(bottomRight.x, bottomRight.y), color);
	}
	else {

		//SDL2 Texture void* is the SDL_Texture*
		SDL_Texture* sdlTexture = TextureManager::instance().getTexture("TEXTURE_IMGUI_ATLAS")->sdlTexture;
		ImGui::Image((void*)(SDL_Texture*)sdlTexture, ImVec2(64, 64), ImVec2(topLeft.x, topLeft.y), ImVec2(bottomRight.x, bottomRight.y), color);
	}

}





