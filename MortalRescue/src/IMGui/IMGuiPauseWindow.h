#include "IMGui/IMGuiItem.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include "GameObject.h"
#include <memory>

class IMGuiPauseWindow : public IMGuiItem
{
public:
	IMGuiPauseWindow() = default;
	IMGuiPauseWindow(Json::Value params, std::string windowName);
	glm::vec2 render(SDL_FRect destRect) override;
	void sendSceneCloseEvent();
	void settingsModal();
	void sendQuitEvent();

	ImFont* m_font{};

private:
	ImVec2 m_settingsModalSize{ 768,384 };
	ImGuiWindowFlags m_SettingsModalflags{ ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings };

};