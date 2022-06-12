#include "IMGuiUtil.h"
#include "../GameConfig.h"
#include "../Renderer.h"
#include "../game.h"

extern std::unique_ptr<Game> game;
namespace ImGui
{
	void MobyDickInit(Game* mobyDickGame)
	{
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.IniFilename = NULL;

		ImGui::StyleColorsDark();

		auto gl_context = SDL_GL_GetCurrentContext();
		const char* glsl_version = "#version 130";

		// Setup Platform/Renderer backends
		if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

			ImGui_ImplSDL2_InitForOpenGL(mobyDickGame->window(), gl_context);
			ImGui_ImplOpenGL3_Init(glsl_version);
		}
		else {
			ImGui_ImplSDL2_InitForSDLRenderer(mobyDickGame->window(), mobyDickGame->renderer()->sdlRenderer());
			ImGui_ImplSDLRenderer_Init(mobyDickGame->renderer()->sdlRenderer());
		}

	}
	void MobyDickNewFrame()
	{

		{
			if (GameConfig::instance().rendererType() == RendererType::OPENGL) {
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplSDL2_NewFrame();
			}
			else {
				ImGui_ImplSDLRenderer_NewFrame();
				ImGui_ImplSDL2_NewFrame();
			}
			ImGui::NewFrame();

		}

	}

	void MobyDickRenderFrame()
	{
		ImGui::Render();
		if (GameConfig::instance().rendererType() == RendererType::OPENGL) {
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
		else {
			ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		}

	}

	void MobyDickFPSFrame()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize = ImVec2{ 4,4 };
		style.WindowPadding = ImVec2{ 0,0 };

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;
		flags |= ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::Begin("FrameRate", nullptr, flags);
		
		auto frate = ImGui::GetIO().Framerate;
		ImGui::Value("FPS", frate);

		//ImGui::PushFont(game->renderer()->font32());
		//ImGui::Text("Todd");
		//ImGui::PopFont();
		//ImGui::Text("Todd2");
		int dwWidth = GameConfig::instance().windowWidth() / 2;
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::End();

	}

	void setFont64()
	{
		//ImGui::PushFont(ImGui::font64);

	}

	void textCentered(std::string text) {
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text(text.c_str());
	}

	void setCursorToCentered(float itemToCenterSize) {
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = itemToCenterSize;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	}

}
