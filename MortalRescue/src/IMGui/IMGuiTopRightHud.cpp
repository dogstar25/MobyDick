#include "IMGuiTopRightHud.h"

#include "ContextManager.h"
#include "Util.h"
#include "game.h"
#include <memory>
#include "../GameConstants.h"
#include "imgui/IMGuiUtil.h"


extern std::unique_ptr<Game> game;

IMGuiTopRightHud::IMGuiTopRightHud(std::string gameObjectType, b2Vec2 padding, Scene* parentScene, ImVec4 backgroundColor, ImVec4 textColor,
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

	SDL_Color yellow = Colors::YELLOW;
	util::colorApplyAlpha(yellow, 255);
	m_hudYellow = util::SDLColorToImVec4(yellow);

	SDL_Color orange = Colors::ORANGE;
	util::colorApplyAlpha(yellow, 255);
	m_hudOrange = util::SDLColorToImVec4(orange);

}

glm::vec2 IMGuiTopRightHud::render()
{

	glm::vec2 windowSize{};

	const auto& renderComponent = parent()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	ImGuiIO& io = ImGui::GetIO(); (void)io;


	//Determine which objective items we have to display
	_adjustForVariableStatusItems();

	setWindowProperties(parent());

	//Set Color

	ImGui::PushStyleColor(ImGuiCol_WindowBg, m_backgroundColor);

	ImGui::Begin(m_gameObjectType.c_str(), nullptr, m_flags);
	{

		//Survivor count display
		if (m_hasSurvivorDisplay) {
			survivorCount();
		}

		//Drone count display
		if (m_hasDroneDisplay) {
			droneCount();
		}

		windowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };

	}
	ImGui::End();

	//pop color style
	ImGui::PopStyleColor();

	return windowSize;

}

void IMGuiTopRightHud::_adjustForVariableStatusItems()
{
	////Do we have a survivor saved objective to display?
	if (game->contextMananger()->getStatusItem(StatusItemId::SURVIVORS).id() != StatusItemId::DEFAULT_EMPTY) {
		m_hasSurvivorDisplay = true;
	}

	////Do we have a drone dead objective to display?
	if (game->contextMananger()->getStatusItem(StatusItemId::ENEMY_DRONE_COUNT).id() != StatusItemId::DEFAULT_EMPTY) {
		m_hasDroneDisplay = true;
	}

	const auto& transformComponent = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	b2Vec2 currentSize = { transformComponent->getPositionRect().w, transformComponent->getPositionRect().h };

	//Adjust size of window depending on what we have to show
	if (m_hasSurvivorDisplay == true && m_hasDroneDisplay == true) {

		transformComponent->setSize(currentSize.x, 224.0);

	}
	else {

		transformComponent->setSize(currentSize.x, 112.0);
	}


}


void IMGuiTopRightHud::survivorCount()
{

	//Survivor image
	hudSurvivorImage(m_hudYellow);

	auto survivorsTotal = game->contextMananger()->getStatusItem(StatusItemId::SURVIVORS).maxValue();
	auto survivorsSaved = game->contextMananger()->getStatusItem(StatusItemId::SURVIVORS).value();

	//Level Text
	std::stringstream levelTxtSS;
	std::string levelTxt;
	levelTxtSS << survivorsSaved << "/" << survivorsTotal;
	levelTxt = levelTxtSS.str();
	ImGui::SameLine();
	ImGui::PushFont(m_largeFont);
	ImGui::Text(levelTxt.c_str());
	ImGui::PopFont();
	ImGui::NewLine();

}

void IMGuiTopRightHud::droneCount()
{

	//drone image
	hudDroneImage(m_hudYellow);

	auto droneTotal = game->contextMananger()->getStatusItem(StatusItemId::ENEMY_DRONE_COUNT).maxValue();
	auto dronesLeft = game->contextMananger()->getStatusItem(StatusItemId::ENEMY_DRONE_COUNT).value();

	//Level Text
	std::stringstream levelTxtSS;
	std::string levelTxt;
	levelTxtSS << dronesLeft << "/" << droneTotal;
	levelTxt = levelTxtSS.str();
	ImGui::SameLine();
	ImGui::PushFont(m_largeFont);
	ImGui::Text(levelTxt.c_str());
	ImGui::PopFont();
	ImGui::NewLine();

}

void IMGuiTopRightHud::hudSurvivorImage(ImVec4 color)
{

	//TextureAtlas Coordinates for bar
	glm::vec2 topLeft = util::glNormalizeTextureCoords({ 65,98 }, { 256, 256 });
	glm::vec2 bottomRight = util::glNormalizeTextureCoords({ 128,161 }, { 256, 256 });

	if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

		GLuint64 textureAtlasId = static_cast<GLRenderer*>(game->renderer())->getTextureId(GL_TextureIndexType::IMGUI_TEXTURE_ATLAS);
		ImGui::Image((void*)textureAtlasId, ImVec2(64, 64), ImVec2(topLeft.x, topLeft.y), ImVec2(bottomRight.x, bottomRight.y), color);
	}
	else {

		//SDL2 Texture void* is the SDL_Texture*
		SDL_Texture* sdlTexture = TextureManager::instance().getTexture("TEXTURE_IMGUI_ATLAS")->sdlTexture;
		ImGui::Image((void*)(SDL_Texture*)sdlTexture, ImVec2(64, 64), ImVec2(topLeft.x, topLeft.y), ImVec2(bottomRight.x, bottomRight.y), color);
	}

}

void IMGuiTopRightHud::hudDroneImage(ImVec4 color)
{

	//TextureAtlas Coordinates for bar
	glm::vec2 topLeft = util::glNormalizeTextureCoords({ 130,98 }, { 256, 256 });
	glm::vec2 bottomRight = util::glNormalizeTextureCoords({ 193,161 }, { 256, 256 });

	if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

		GLuint64 textureAtlasId = static_cast<GLRenderer*>(game->renderer())->getTextureId(GL_TextureIndexType::IMGUI_TEXTURE_ATLAS);
		ImGui::Image((void*)(int*)textureAtlasId, ImVec2(64, 64), ImVec2(topLeft.x, topLeft.y), ImVec2(bottomRight.x, bottomRight.y), color);
	}
	else {

		//SDL2 Texture void* is the SDL_Texture*
		SDL_Texture* sdlTexture = TextureManager::instance().getTexture("TEXTURE_IMGUI_ATLAS")->sdlTexture;
		ImGui::Image((void*)(SDL_Texture*)sdlTexture, ImVec2(64, 64), ImVec2(topLeft.x, topLeft.y), ImVec2(bottomRight.x, bottomRight.y), color);
	}

}

