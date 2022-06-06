#include "IMGuiTopHud.h"

#include "ContextManager.h"
#include "Util.h"
#include "game.h"
#include <memory>
#include "../GameConstants.h"
#include "imgui/IMGuiUtil.h"


extern std::unique_ptr<Game> game;

IMGuiTopHud::IMGuiTopHud(std::string gameObjectId, b2Vec2 padding, Scene* parentScene, ImVec4 backgroundColor, ImVec4 textColor,
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


	testObject = std::make_shared<GameObject>("SWORDLADY", -1.0F, -1.0F, 0.F, parentScene);


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
		ImGui::NewLine();
		weaponLevel();
		ImGui::NewLine();
		hudWeaponPistolImage(m_hudRed);

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

	auto& livesStatusItem = game->contextMananger()->getStatusItem("LIVES_COUNT");

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


void IMGuiTopHud::hudScrapCount()
{

	ImGui::Text("%s  %d", "ScrapCount", (int)game->contextMananger()->getValue("SCRAP_COUNT"));

}

void IMGuiTopHud::weaponLevel()
{

	ImGui::NewLine();

	ImVec4 gunColor{};

	//TextureAtlas Coordinates

	//Get the value for the current player weapon levelup accrual
	auto& levelUpAccrual = game->contextMananger()->getStatusItem("PLAYER_WEAPON_LEVEL_ACCRUAL");

	//Determine color of weapon
	auto& weaponLevel = game->contextMananger()->getStatusItem("PLAYER_WEAPON_LEVEL");
	if (weaponLevel.value() == 1) {
		gunColor = m_hudBlue;
	}
	else if (weaponLevel.value() == 2) {
		gunColor = m_hudGreen;
	}
	else if (weaponLevel.value() == 3) {
		gunColor = m_hudRed;
	}

	//Pistol image
	hudWeaponPistolImage(gunColor);

	//Level Text
	std::stringstream levelTxtSS;
	std::string levelTxt;
	levelTxtSS << "LVL" << weaponLevel.value();
	levelTxt = levelTxtSS.str();
	ImGui::SameLine();
	//ImGui::setFont64();
	ImGui::PushFont(game->renderer()->font64());

	//ImGui::Dummy(ImVec2(0.0f, 32.0f));
	ImGui::Text(levelTxt.c_str());
	ImGui::PopFont();
	ImGui::NewLine();
	//Bar grapgh showing the accrual level
	hudWeaponAccrualBar(gunColor, levelUpAccrual.value());

}

void IMGuiTopHud::hudWeaponPistolImage(ImVec4 color)
{

	//const auto& transform = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	//testObject->setPosition(transform->getTopLeftPosition());
	//testObject->render();

	//TextureAtlas Coordinates for bar
	glm::vec2 topLeft = util::glNormalizeTextureCoords({ 0,98 }, { 256, 256 });
	glm::vec2 bottomRight = util::glNormalizeTextureCoords({ 63,161 }, { 256, 256 });

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

void IMGuiTopHud::hudWeaponAccrualBar(ImVec4 color, float accrualValue)
{
	//CurrLineTextBaseOffset
	ImGui::SameLine();

	//TextureAtlas Coordinates for bar
	glm::vec2 topLeft = util::glNormalizeTextureCoords({ 33,0 }, { 256, 256 });
	glm::vec2 bottomRight = util::glNormalizeTextureCoords({ 63,31 }, { 256, 256 });

	for (int i = 0; i < accrualValue; i++) {

		if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

			GLuint textureAtlasId = static_cast<GLRenderer*>(game->renderer())->getTextureId(GL_TextureIndexType::IMGUI_TEXTURE_ATLAS);
			ImGui::Image((void*)(int*)textureAtlasId, ImVec2(2, 32), ImVec2(bottomRight.x, bottomRight.y), ImVec2(topLeft.x, topLeft.y), color);
			ImGui::SameLine(0.0f, 0);
		}
		else {

			//SDL2 Texture void* is the SDL_Texture*
			SDL_Texture* sdlTexture = TextureManager::instance().getTexture("TEXTURE_IMGUI_ATLAS")->sdlTexture;
			ImGui::Image((void*)(SDL_Texture*)sdlTexture, ImVec2(2, 32), ImVec2(topLeft.x, topLeft.y), ImVec2(bottomRight.x, bottomRight.y), color);
			ImGui::SameLine(0.0f, 0);
		}

	}

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

